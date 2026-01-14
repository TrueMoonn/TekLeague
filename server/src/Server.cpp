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

#include <ECS/DenseSA.hpp>
#include <ECS/DenseZipper.hpp>

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
}

bool Server::setPacketsHandlers() {
    registerPacketHandler(2, [this](const std::vector<uint8_t>& data, const net::Address& sender) {
        handleDisconnection(data, sender);
    });
    registerPacketHandler(20, [this](const std::vector<uint8_t>& data, const net::Address& sender) {
        handleLogin(data, sender);
    });
    registerPacketHandler(21, [this](const std::vector<uint8_t>& data, const net::Address& sender) {
        handleLogout(data, sender);
    });
    registerPacketHandler(30, [this](const std::vector<uint8_t>& data, const net::Address& sender) {
        handleJoinLobby(data, sender);
    });
    registerPacketHandler(32, [this](const std::vector<uint8_t>& data, const net::Address& sender) {
        handleCreateLobby(data, sender);
    });
    registerPacketHandler(34, [this](const std::vector<uint8_t>& data, const net::Address& sender) {
        handleGetAllPublicLobbies(data, sender);
    });
    registerPacketHandler(36, [this](const std::vector<uint8_t>& data, const net::Address& sender) {
        handleAdminStartGame(data, sender);
    });
    registerPacketHandler(40, [this](const std::vector<uint8_t>& data, const net::Address& sender) {
        handleLeaveLobby(data, sender);
    });
    registerPacketHandler(46, [this](const std::vector<uint8_t>& data, const net::Address& sender) {
        handleToggleLobbyVisibility(data, sender);
    });
    registerPacketHandler(87, [this](const std::vector<uint8_t>& data, const net::Address& sender) {
        handleAdminPauseGame(data, sender);
    });
    return true;
}

uint32_t Server::generateClientId() {
    return next_client_id++;
}

std::optional<std::reference_wrapper<PlayerInfo>> Server::getClient(const net::Address& address) {
    auto it = clients.find(address);
    if (it != clients.end()) {
        return std::ref(it->second);
    }
    return std::nullopt;
}

std::optional<std::reference_wrapper<PlayerInfo>> Server::getClientById(uint32_t client_id) {
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

    std::println("[Server::createLobby] Creating lobby {} with code {} for admin {}:{}",
                 lobby_id, code, admin.getIP(), admin.getPort());

    lobby_codes[code] = lobby_id;
    lobbies.try_emplace(lobby_id, max_clients, code);
    lobby_admins[lobby_id] = admin;
    public_lobbies.insert(lobby_id);  // Default to public

    std::println("[Server::createLobby] Lobby created successfully. Total lobbies: {}", lobbies.size());

    return lobby_id;
}

void Server::destroyLobby(uint lobby_id) {
    std::lock_guard<std::mutex> lock(lobbies_mutex);
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

void Server::broadcastToLobbyUnsafe(uint lobby_id, const std::vector<uint8_t>& data) {
    if (lobbies.find(lobby_id) == lobbies.end()) {
        std::println("[Server] broadcastToLobby: Lobby {} not found!", lobby_id);
        return;
    }

    auto& lobby_clients = lobbies.at(lobby_id).getClients();

    for (const auto& [client_id, address] : lobby_clients) {
        sendTo(address, data);
    }
}

void Server::sendToLobby(uint lobby_id, const std::vector<uint8_t>& data, const net::Address& exclude) {
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

void Server::handleDisconnection(const std::vector<uint8_t>& data, const net::Address& sender) {
    auto client_opt = getClient(sender);
    if (!client_opt)
        return;

    auto& client = client_opt->get();

    if (client.in_lobby) {
        handleLeaveLobby(data, sender);
    }

    if (!client.username.empty()) {
        usernames.erase(client.username);
    }

    clients.erase(sender);
}

void Server::handleLogin(const std::vector<uint8_t>& data, const net::Address& sender) {
    auto msg = net::LOGIN::deserialize(data);
    std::string username(msg.username, strnlen(msg.username, 32));

    if (!isUsernameAvailable(username)) {
        sendUsernameAlreadyTaken(sender);
        return;
    }

    auto client_opt = getClient(sender);
    if (!client_opt) {
        PlayerInfo new_client;
        new_client.id = generateClientId();
        new_client.username = username;
        new_client.address = sender;
        clients[sender] = new_client;
        client_opt = getClient(sender);
    } else {
        auto& client = client_opt->get();
        if (!client.username.empty()) {
            usernames.erase(client.username);
        }
        client.username = username;
    }

    auto& client = client_opt->get();
    usernames[username] = sender;
    sendLoggedIn(sender, client.id);
}

void Server::handleLogout(const std::vector<uint8_t>& data, const net::Address& sender) {
    auto client_opt = getClient(sender);
    if (!client_opt) return;

    auto& client = client_opt->get();
    uint32_t client_id = client.id;

    if (!client.username.empty()) {
        usernames.erase(client.username);
        client.username.clear();
    }

    sendLoggedOut(sender, client_id);
}

void Server::handleJoinLobby(const std::vector<uint8_t>& data, const net::Address& sender) {
    auto msg = net::JOIN_LOBBY::deserialize(data);
    std::string lobby_code(msg.lobby_code, 6);

    std::println("[Server::handleJoinLobby] Request from {}:{}", sender.getIP(), sender.getPort());
    std::println("[Server::handleJoinLobby] Lobby code: '{}'", lobby_code);

    auto client_opt = getClient(sender);
    if (!client_opt) {
        std::println("[Server::handleJoinLobby] Client not found");
        return;
    }

    auto& client = client_opt->get();
    std::println("[Server::handleJoinLobby] Client {} ({}) joining lobby", client.id, client.username);

    uint lobby_id;
    {
        std::lock_guard<std::mutex> lock(lobbies_mutex);
        auto it = lobby_codes.find(lobby_code);
        if (it == lobby_codes.end()) {
            std::println("[Server::handleJoinLobby] Lobby code not found");
            sendBadLobbyCode(sender);
            return;
        }

        lobby_id = it->second;
        std::println("[Server::handleJoinLobby] Found lobby ID: {}", lobby_id);

        auto& lobby_ctx = lobbies.at(lobby_id);

        if (lobby_ctx.isFull()) {
            std::println("[Server::handleJoinLobby] Lobby is full");
            sendLobbyFull(sender);
            return;
        }

        if (!lobby_ctx.addClient(sender, client.id)) {
            std::println("[Server::handleJoinLobby] Failed to add client to lobby");
            sendBadLobbyCode(sender);
            return;
        }

        std::println("[Server::handleJoinLobby] Client added to lobby successfully");
    }

    client.in_lobby = true;
    client.lobby_id = lobby_id;

    std::println("[Server::handleJoinLobby] Sending LOBBY_JOINED to client...");
    sendLobbyJoined(sender, client.id);

    std::println("[Server::handleJoinLobby] Sending PLAYERS_LIST to all clients in lobby...");
    sendPlayersList(lobby_id);
    std::println("[Server::handleJoinLobby] Completed for lobby {}", lobby_id);
}

void Server::handleCreateLobby(const std::vector<uint8_t>& data, const net::Address& sender) {
    std::println("[Server::handleCreateLobby] Request from {}:{}", sender.getIP(), sender.getPort());

    auto client_opt = getClient(sender);
    if (!client_opt) {
        std::println("[Server::handleCreateLobby] Client not found");
        return;
    }

    auto& client = client_opt->get();
    std::println("[Server::handleCreateLobby] Client {} ({}) creating lobby", client.id, client.username);

    uint lobby_id = createLobby(10, sender);

    std::string lobby_code;
    {
        std::lock_guard<std::mutex> lock(lobbies_mutex);
        auto& lobby_ctx = lobbies.at(lobby_id);

        lobby_ctx.addClient(sender, client.id);
        client.in_lobby = true;
        client.lobby_id = lobby_id;

        lobby_code = lobby_ctx.getCode();
    }

    std::println("[Server::handleCreateLobby] Sending LOBBY_CREATED...");
    sendLobbyCreated(sender, lobby_code);
    std::println("[Server::handleCreateLobby] Sending PLAYERS_LIST...");
    sendPlayersList(lobby_id);
    std::println("[Server::handleCreateLobby] Completed for lobby {}", lobby_id);
}

void Server::handleGetAllPublicLobbies(const std::vector<uint8_t>& data, const net::Address& sender) {
    sendLobbiesList(sender);
}

void Server::handleAdminStartGame(const std::vector<uint8_t>& data, const net::Address& sender) {
    std::println("[Server] handleAdminStartGame: Request from {}:{}", sender.getIP(), sender.getPort());

    auto client_opt = getClient(sender);
    if (!client_opt) {
        std::println("[Server] handleAdminStartGame: Client not found");
        return;
    }

    auto& client = client_opt->get();
    if (!client.in_lobby) {
        std::println("[Server] handleAdminStartGame: Client not in lobby");
        return;
    }

    uint lobby_id = client.lobby_id;
    std::println("[Server] handleAdminStartGame: Client {} is in lobby {}", client.id, lobby_id);

    if (!isAdmin(sender, lobby_id)) {
        std::println("[Server] handleAdminStartGame: User is not admin");
        sendNotAdmin(sender);
        return;
    }

    std::println("[Server] handleAdminStartGame: Starting game for lobby {}", lobby_id);
    sendGameStarting(lobby_id);
    std::println("[Server] handleAdminStartGame: sendGameStarting completed");

    // TODO(Pierre): Initialize game state in lobby
    // std::lock_guard<std::mutex> lock(lobbies_mutex);
    // lobbies.at(lobby_id).getLobby().startGame();
}

void Server::handleLeaveLobby(const std::vector<uint8_t>& data, const net::Address& sender) {
    auto client_opt = getClient(sender);
    if (!client_opt)
        return;

    auto& client = client_opt->get();
    if (!client.in_lobby)
        return;

    uint lobby_id = client.lobby_id;

    bool lobby_is_empty = false;
    {
        std::lock_guard<std::mutex> lock(lobbies_mutex);
        auto& lobby_ctx = lobbies.at(lobby_id);
        lobby_ctx.removeClient(client.id);

        lobby_is_empty = lobby_ctx.getClients().empty();

        // If admin left and lobby not empty, assign new admin
        if (!lobby_is_empty && isAdmin(sender, lobby_id)) {
            const auto& clients_in_lobby = lobby_ctx.getClients();
            if (!clients_in_lobby.empty()) {
                lobby_admins[lobby_id] = clients_in_lobby.begin()->second;
            }
        }
    }

    client.in_lobby = false;
    client.lobby_id = 0;

    if (lobby_is_empty) {
        destroyLobby(lobby_id);
        return;
    }

    sendPlayersList(lobby_id);
}

void Server::handleToggleLobbyVisibility(const std::vector<uint8_t>& data, const net::Address& sender) {
    std::println("[Server] handleToggleLobbyVisibility: Request from {}:{}", sender.getIP(), sender.getPort());

    auto client_opt = getClient(sender);
    if (!client_opt)
        return;

    auto& client = client_opt->get();
    if (!client.in_lobby)
        return;

    uint lobby_id = client.lobby_id;

    if (!isAdmin(sender, lobby_id)) {
        std::println("[Server] handleToggleLobbyVisibility: User is not admin");
        sendNotAdmin(sender);
        return;
    }

    bool is_public;
    {
        std::lock_guard<std::mutex> lock(lobbies_mutex);
        if (public_lobbies.find(lobby_id) != public_lobbies.end()) {
            public_lobbies.erase(lobby_id);
            is_public = false;
        } else {
            public_lobbies.insert(lobby_id);
            is_public = true;
        }
    }

    std::println("[Server] handleToggleLobbyVisibility: Lobby {} is now {}",
                 lobby_id, (is_public ? "PUBLIC" : "PRIVATE"));
    sendLobbyVisibilityChanged(lobby_id, is_public);
}

void Server::handleAdminPauseGame(const std::vector<uint8_t>& data, const net::Address& sender) {
    std::println("[Server] handleAdminPauseGame: Request from {}:{}", sender.getIP(), sender.getPort());

    auto client_opt = getClient(sender);
    if (!client_opt)
        return;

    auto& client = client_opt->get();
    if (!client.in_lobby)
        return;

    uint lobby_id = client.lobby_id;

    if (!isAdmin(sender, lobby_id)) {
        std::println("[Server] handleAdminPauseGame: User is not admin");
        sendNotAdmin(sender);
        return;
    }

    // TODO(Pierre): Toggle pause state in lobby
    // lobbies.at(lobby_id).getLobby().togglePause();

    std::println("[Server] handleAdminPauseGame: Toggling pause for lobby {}", lobby_id);
    sendAdminGamePaused(lobby_id);
}

void Server::sendAutomatic() {
    try {
        std::vector<std::pair<uint, std::vector<uint8_t>>> messages_to_send;
        {
            std::lock_guard<std::mutex> lock(lobbies_mutex);
            for (auto& [id, ctx] : lobbies) {
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

void Server::sendLobbyCreated(const net::Address& address, const std::string& lobby_code) {
    net::LOBBY_CREATED msg;
    std::memcpy(msg.lobby_code, lobby_code.c_str(), std::min(lobby_code.size(), size_t(6)));
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
    std::println("[Server] sendGameStarting: Broadcasting GAME_STARTING to lobby {}", lobby_id);
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

    std::println("[Server::sendPlayersList] Broadcasting to lobby with {} players", msg.players.size());
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

void Server::run() {
    std::println("[Server::run] Starting main loop");
    int loop_count = 0;

    try {
        while (isRunning() && _should_run.load()) {
            if (!_should_run.load()) {
                std::println("[Server::run] Shutdown signal received, exiting loop");
                break;
            }

            // Run game logic for each lobby
            try {
                std::lock_guard<std::mutex> lock(lobbies_mutex);
                if (!lobbies.empty()) {
                    // Only log every 1000 iterations to reduce spam
                    if (loop_count % 1000 == 0) {
                        std::println("[Server::run] Running {} lobby contexts", lobbies.size());
                    }
                    for (auto& [id, ctx] : lobbies) {
                        // TODO(PIERRE): Uncomment when Lobby scenes/systems are properly initialized
                        // ctx.run();
                    }
                }
            } catch (const std::exception& e) {
                std::println(stderr, "[Server::run] ERROR in lobby loop: {}", e.what());
                throw;
            }

            try {
                update(0);
            } catch (const std::exception& e) {
                std::println(stderr, "[Server::run] ERROR in update: {}", e.what());
                throw;
            }

            try {
                sendAutomatic();
            } catch (const std::exception& e) {
                std::println(stderr, "[Server::run] ERROR in sendAutomatic: {}", e.what());
                throw;
            }

            // Small sleep to avoid burning CPU
            std::this_thread::sleep_for(std::chrono::milliseconds(1));
            loop_count++;
        }
    } catch (const std::exception& e) {
        std::println(stderr, "[Server::run] FATAL ERROR in main loop: {}", e.what());
        throw;
    }

    std::println("[Server::run] Exited main loop after {} iterations", loop_count);
}

void Server::stop() {
    std::println("[Server] Stopping server...");
    _should_run.store(false);
    stopGameServer();
}
