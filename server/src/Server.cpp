/*
** EPITECH PROJECT, 2025
** TekLeague
** File description:
** Server.cpp
*/

#include <cstdint>
#include <string>
#include <vector>
#include <random>
#include <thread>
#include <chrono>
#include <print>
#include <algorithm>

#include <Network/generated_messages.hpp>

#include <physic/components/position.hpp>
#include <entity_spec/components/health.hpp>
#include <entity_spec/components/team.hpp>
#include <interaction/components/player.hpp>

#include <network/GameServer.hpp>

#include "Server.hpp"

Server::Server(uint16_t port, const std::string& protocol) :
    te::network::GameServer(port, protocol) {
    startGameServer();
    setPacketsHandlers();

    setClientDisconnectCallback([this](const net::Address& client) {
        std::println("[Server] Client disconnected or timed out: {}:{}",
            client.getIP(), client.getPort());

        std::vector<uint8_t> empty_data;
        handleDisconnection(empty_data, client);
    });
}

uint32_t Server::generateClientId() {
    return next_client_id++;
}

std::optional<std::reference_wrapper<PlayerInfo>> Server::getClient(
    const net::Address& address) {
    auto it = clients.find(address);
    if (it != clients.end()) {
        return std::ref(it->second);
    }
    return std::nullopt;
}

std::optional<std::reference_wrapper<PlayerInfo>> Server::getClientById(
    uint32_t client_id) {
    for (auto& [addr, client] : clients) {
        if (client.id == client_id) {
            return std::ref(client);
        }
    }
    return std::nullopt;
}

bool Server::isUsernameAvailable(const std::string& username) {
    return usernames.find(username) == usernames.end();
}

bool Server::isAdmin(const net::Address& address, uint lobby_id) {
    auto it = lobby_admins.find(lobby_id);
    if (it == lobby_admins.end())
        return false;
    return it->second == address;
}

uint Server::createLobby(uint max_clients, const net::Address& admin) {
    std::lock_guard<std::mutex> lock(lobbies_mutex);
    std::string code = generateUniqueLobbyCode();
    uint lobby_id = next_lobby_id++;

    std::println(
        "[Server::createLobby] Creating lobby {} with code {} for admin {}:{}",
                 lobby_id, code, admin.getIP(), admin.getPort());

    lobby_codes[code] = lobby_id;
    lobbies.try_emplace(lobby_id, max_clients, code);
    lobby_admins[lobby_id] = admin;
    public_lobbies.insert(lobby_id);  // Default to public

    std::println(
        "[Server::createLobby] Lobby created successfully. Total lobbies: {}",
        lobbies.size());

    return lobby_id;
}

void Server::destroyLobby(uint lobby_id) {
    std::lock_guard<std::mutex> lock(lobbies_mutex);
    destroyLobbyUnsafe(lobby_id);
}

void Server::destroyLobbyUnsafe(uint lobby_id) {
    if (lobbies.find(lobby_id) != lobbies.end()) {
        sendLobbyDestroyed(lobby_id);

        for (auto& [client_id, address] : lobbies.at(lobby_id).getClients()) {
            if (auto client_opt = getClient(address)) {
                auto& client = client_opt->get();
                client.in_lobby = false;
                client.lobby_id = 0;
            }
        }

        std::string code = lobbies.at(lobby_id).getCode();
        lobby_codes.erase(code);
        lobby_admins.erase(lobby_id);
        lobbies.erase(lobby_id);
        public_lobbies.erase(lobby_id);
    }
}

void Server::broadcastToLobby(uint lobby_id, const std::vector<uint8_t>& data) {
    std::lock_guard<std::mutex> lock(lobbies_mutex);
    broadcastToLobbyUnsafe(lobby_id, data);
}

void Server::broadcastToLobbyUnsafe(
    uint lobby_id, const std::vector<uint8_t>& data) {
    if (lobbies.find(lobby_id) == lobbies.end()) {
        std::println("[Server] broadcastToLobby: Lobby {} not found!",
            lobby_id);
        return;
    }

    auto& lobby_clients = lobbies.at(lobby_id).getClients();

    for (const auto& [client_id, address] : lobby_clients) {
        sendTo(address, data);
    }
}

void Server::sendToLobby(uint lobby_id, const std::vector<uint8_t>& data,
    const net::Address& exclude) {
    std::lock_guard<std::mutex> lock(lobbies_mutex);
    if (lobbies.find(lobby_id) == lobbies.end())
        return;

    for (const auto& [client_id, address] : lobbies.at(lobby_id).getClients()) {
        if (address != exclude) {
            sendTo(address, data);
        }
    }
}

std::string Server::generateUniqueLobbyCode() {
    static const char alphanum[] =
        "0123456789"
        "ABCDEFGHIJKLMNOPQRSTUVWXYZ"
        "abcdefghijklmnopqrstuvwxyz";
    static const int alphanum_size = sizeof(alphanum) - 1;

    static std::random_device rd;
    static std::mt19937 gen(rd());
    static std::uniform_int_distribution<> dis(0, alphanum_size - 1);

    std::string code;
    const int CODE_LENGTH = 6;

    do {
        code.clear();
        for (int i = 0; i < CODE_LENGTH; ++i) {
            code += alphanum[dis(gen)];
        }
    } while (lobby_codes.find(code) != lobby_codes.end());

    return code;
}

void Server::run() {
    std::println("[Server::run] Starting main loop");
    int loop_count = 0;

    try {
        while (isRunning() && _should_run.load()) {
            if (!_should_run.load()) {
                std::println(
                    "[Server::run] Shutdown signal received, exiting loop");
                break;
            }

            // Run game logic for each lobby
            try {
                std::lock_guard<std::mutex> lock(lobbies_mutex);
                if (!lobbies.empty()) {
                    // Only log every 1000 iterations to reduce spam
                    if (loop_count % 1000 == 0) {
                        std::println(
                            "[Server::run] Running {} lobby contexts",
                            lobbies.size());
                    }
                    for (auto& [id, ctx] : lobbies) {
                        ctx.run();
                    }
                }
            } catch (const std::exception& e) {
                std::println(stderr,
                    "[Server::run] ERROR in lobby loop: {}", e.what());
                throw;
            }

            try {
                update(0);
            } catch (const std::exception& e) {
                std::println(stderr,
                    "[Server::run] ERROR in update: {}", e.what());
                throw;
            }

            try {
                sendAutomatic();
            } catch (const std::exception& e) {
                std::println(stderr,
                    "[Server::run] ERROR in sendAutomatic: {}", e.what());
                throw;
            }

            // Small sleep to avoid burning CPU
            std::this_thread::sleep_for(std::chrono::milliseconds(1));
            loop_count++;
        }
    } catch (const std::exception& e) {
        std::println(stderr,
            "[Server::run] FATAL ERROR in main loop: {}", e.what());
        throw;
    }

    std::println("[Server::run] Exited main loop after {} iterations",
        loop_count);
}

void Server::stop() {
    std::println("[Server] Stopping server...");
    _should_run.store(false);
    stopGameServer();
}

std::optional<LobbyGameState> Server::getLobbyGameState(uint lobby_id) {
    std::lock_guard<std::mutex> lock(lobbies_mutex);
    if (lobbies.find(lobby_id) != lobbies.end()) {
        return lobbies.at(lobby_id).getGameState();
    }
    return std::nullopt;
}

bool Server::isLobbyPreGame(uint lobby_id) {
    std::lock_guard<std::mutex> lock(lobbies_mutex);
    if (lobbies.find(lobby_id) != lobbies.end()) {
        return lobbies.at(lobby_id).isPreGame();
    }
    return false;
}

bool Server::isLobbyInGame(uint lobby_id) {
    std::lock_guard<std::mutex> lock(lobbies_mutex);
    if (lobbies.find(lobby_id) != lobbies.end()) {
        return lobbies.at(lobby_id).isInGame();
    }
    return false;
}

bool Server::isLobbyEndGame(uint lobby_id) {
    std::lock_guard<std::mutex> lock(lobbies_mutex);
    if (lobbies.find(lobby_id) != lobbies.end()) {
        return lobbies.at(lobby_id).isEndGame();
    }
    return false;
}
