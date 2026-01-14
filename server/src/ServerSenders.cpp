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

#include <ECS/DenseSA.hpp>
#include <ECS/DenseZipper.hpp>

#include <Network/generated_messages.hpp>

#include <physic/components/position.hpp>
#include <entity_spec/components/health.hpp>
#include <entity_spec/components/team.hpp>
#include <interaction/components/player.hpp>

#include <network/GameServer.hpp>

#include "Server.hpp"
#include "lobby/LobbyContext.hpp"

void Server::sendAutomatic() {
    try {
        std::vector<std::pair<uint, std::vector<uint8_t>>> messages_to_send;
        {
            std::lock_guard<std::mutex> lock(lobbies_mutex);
            for (auto& [id, ctx] : lobbies) {
                if (ctx.getGameState() != LobbyGameState::IN_GAME)
                    continue;
                if (auto msg = ctx.tryGetPlayerUpdates()) {
                    messages_to_send.emplace_back(id, msg->serialize());
                }
                if (auto msg = ctx.tryGetPlayerUpdates()) {
                    messages_to_send.emplace_back(id, msg->serialize());
                }
            }
        }

        for (const auto& [id, data] : messages_to_send) {
            broadcastToLobby(id, data);
        }
    } catch (const std::exception& e) {
        std::println(stderr, "[Server::sendAutomatic] ERROR: {}", e.what());
    }
}

void Server::sendPlayersUpdate() {
    try {
        std::lock_guard<std::mutex> lock(lobbies_mutex);
        for (auto& [id, ctx] : lobbies) {
            if (auto msg = ctx.tryGetPlayerUpdates()) {
                if (!msg->players.empty()) {
                    broadcastToLobbyUnsafe(id, msg->serialize());
                }
            }
        }
    } catch (const std::exception& e) {
        std::println(stderr, "[Server::sendPlayersUpdate] ERROR: {}", e.what());
    }
}

void Server::sendLoggedIn(const net::Address& address, uint32_t client_id) {
    net::LOGGED_IN msg;
    msg.id = client_id;
    sendTo(address, msg.serialize());
}

void Server::sendLoggedOut(const net::Address& address, uint32_t client_id) {
    net::LOGGED_OUT msg;
    msg.id = client_id;
    sendTo(address, msg.serialize());
}

void Server::sendUsernameAlreadyTaken(const net::Address& address) {
    net::USERNAME_ALREADY_TAKEN msg;
    sendTo(address, msg.serialize());
}

void Server::sendLobbyJoined(const net::Address& address, uint32_t client_id) {
    net::LOBBY_JOINED msg;
    msg.id = client_id;
    sendTo(address, msg.serialize());
}

void Server::sendLobbyCreated(
    const net::Address& address, const std::string& lobby_code) {
    net::LOBBY_CREATED msg;
    std::memcpy(msg.lobby_code, lobby_code.c_str(),
        std::min(lobby_code.size(), size_t(6)));
    sendTo(address, msg.serialize());
}

void Server::sendLobbiesList(const net::Address& address) {
    net::LOBBIES_LIST msg;

    {
        std::lock_guard<std::mutex> lock(lobbies_mutex);
        for (uint lobby_id : public_lobbies) {
            if (lobbies.find(lobby_id) != lobbies.end()) {
                msg.lobby_codes.push_back(lobbies.at(lobby_id).getCode());
            }
        }
    }

    sendTo(address, msg.serialize());
}

void Server::sendGameStarting(uint lobby_id) {
    std::println(
        "[Server] sendGameStarting: Broadcasting GAME_STARTING to lobby {}",
        lobby_id);
    net::GAME_STARTING msg;
    auto serialized = msg.serialize();
    broadcastToLobby(lobby_id, serialized);
    std::println("[Server] sendGameStarting: Broadcast complete");
}

void Server::sendPlayersList(uint lobby_id) {
    std::println("[Server::sendPlayersList] Entering for lobby {}", lobby_id);
    std::lock_guard<std::mutex> lock(lobbies_mutex);
    std::println("[Server::sendPlayersList] Lock acquired");

    if (lobbies.find(lobby_id) == lobbies.end()) {
        std::println("[Server::sendPlayersList] Lobby not found");
        return;
    }

    net::PLAYERS_LIST msg;

    for (const auto& [client_id, address] : lobbies.at(lobby_id).getClients()) {
        auto client_opt = getClient(address);
        if (client_opt) {
            auto& client = client_opt->get();
            net::PlayerListEntry entry;
            entry.id = client.id;
            std::memset(entry.username, 0, 32);
            std::memcpy(entry.username, client.username.c_str(),
                std::min(client.username.size(), size_t(32)));
            msg.players.push_back(entry);
        }
    }

    std::println(
        "[Server::sendPlayersList] Broadcasting to lobby with {} players",
        msg.players.size());
    broadcastToLobbyUnsafe(lobby_id, msg.serialize());
    std::println("[Server::sendPlayersList] Completed");
}

void Server::sendLobbyVisibilityChanged(uint lobby_id, bool is_public) {
    net::LOBBY_VISIBILITY_CHANGED msg;
    msg.is_public = is_public ? 1 : 0;
    broadcastToLobby(lobby_id, msg.serialize());
}

void Server::sendLobbyDestroyed(uint lobby_id) {
    net::LOBBY_DESTROYED msg;
    broadcastToLobby(lobby_id, msg.serialize());
}

void Server::sendLobbyFull(const net::Address& address) {
    net::LOBBY_FULL msg;
    sendTo(address, msg.serialize());
}

void Server::sendBadLobbyCode(const net::Address& address) {
    net::BAD_LOBBY_CODE msg;
    sendTo(address, msg.serialize());
}

void Server::sendNotAdmin(const net::Address& address) {
    net::NOT_ADMIN msg;
    sendTo(address, msg.serialize());
}

void Server::sendAdminGamePaused(uint lobby_id) {
    net::ADMIN_GAME_PAUSED msg;
    broadcastToLobby(lobby_id, msg.serialize());
}

void Server::sendGameEnded(uint lobby_id) {
    std::println(
        "[Server] sendGameEnded: Broadcasting GAME_END to lobby {}",
        lobby_id);
    net::GAME_END msg;
    msg.winning_team = 0;  // TODO: Determine winning team
    auto serialized = msg.serialize();
    broadcastToLobby(lobby_id, serialized);
    std::println("[Server] sendGameEnded: Broadcast complete");
}
