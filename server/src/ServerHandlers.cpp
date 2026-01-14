/*
** EPITECH PROJECT, 2025
** TekLeague
** File description:
** Server.cpp
*/

#include <cstdint>
#include <string>
#include <vector>
#include <print>

#include <network/GameServer.hpp>

#include "Server.hpp"

bool Server::setPacketsHandlers() {
    registerPacketHandler(2, [this](const std::vector<uint8_t>& data,
        const net::Address& sender) {
        handleDisconnection(data, sender);
    });
    registerPacketHandler(20, [this](const std::vector<uint8_t>& data,
        const net::Address& sender) {
        handleLogin(data, sender);
    });
    registerPacketHandler(21, [this](const std::vector<uint8_t>& data,
        const net::Address& sender) {
        handleLogout(data, sender);
    });
    registerPacketHandler(30, [this](const std::vector<uint8_t>& data,
        const net::Address& sender) {
        handleJoinLobby(data, sender);
    });
    registerPacketHandler(32, [this](const std::vector<uint8_t>& data,
        const net::Address& sender) {
        handleCreateLobby(data, sender);
    });
    registerPacketHandler(34, [this](const std::vector<uint8_t>& data,
        const net::Address& sender) {
        handleGetAllPublicLobbies(data, sender);
    });
    registerPacketHandler(36, [this](const std::vector<uint8_t>& data,
        const net::Address& sender) {
        handleAdminStartGame(data, sender);
    });
    registerPacketHandler(40, [this](const std::vector<uint8_t>& data,
        const net::Address& sender) {
        handleLeaveLobby(data, sender);
    });
    registerPacketHandler(46, [this](const std::vector<uint8_t>& data,
        const net::Address& sender) {
        handleToggleLobbyVisibility(data, sender);
    });
    registerPacketHandler(87, [this](const std::vector<uint8_t>& data,
        const net::Address& sender) {
        handleAdminPauseGame(data, sender);
    });
    return true;
}

void Server::handleDisconnection(const std::vector<uint8_t>& data,
    const net::Address& sender) {
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

void Server::handleLogin(const std::vector<uint8_t>& data,
    const net::Address& sender) {
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

void Server::handleLogout(const std::vector<uint8_t>& data,
    const net::Address& sender) {
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

void Server::handleJoinLobby(const std::vector<uint8_t>& data,
    const net::Address& sender) {
    auto msg = net::JOIN_LOBBY::deserialize(data);
    std::string lobby_code(msg.lobby_code, 6);

    std::println("[Server::handleJoinLobby] Request from {}:{}",
        sender.getIP(), sender.getPort());
    std::println("[Server::handleJoinLobby] Lobby code: '{}'", lobby_code);

    auto client_opt = getClient(sender);
    if (!client_opt) {
        std::println("[Server::handleJoinLobby] Client not found");
        return;
    }

    auto& client = client_opt->get();
    std::println("[Server::handleJoinLobby] Client {} ({}) joining lobby",
        client.id, client.username);

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
            std::println(
                "[Server::handleJoinLobby] Failed to add client to lobby");
            sendBadLobbyCode(sender);
            return;
        }

        std::println(
            "[Server::handleJoinLobby] Client added to lobby successfully");
    }

    client.in_lobby = true;
    client.lobby_id = lobby_id;

    std::println("[Server::handleJoinLobby] Sending LOBBY_JOINED to client...");
    sendLobbyJoined(sender, client.id);

    std::println(
    "[Server::handleJoinLobby] Sending PLAYERS_LIST to all clients in lobby");
    sendPlayersList(lobby_id);
    std::println("[Server::handleJoinLobby] Completed for lobby {}", lobby_id);
}

void Server::handleCreateLobby(const std::vector<uint8_t>& data,
    const net::Address& sender) {
    std::println("[Server::handleCreateLobby] Request from {}:{}",
        sender.getIP(), sender.getPort());

    auto client_opt = getClient(sender);
    if (!client_opt) {
        std::println("[Server::handleCreateLobby] Client not found");
        return;
    }

    auto& client = client_opt->get();
    std::println("[Server::handleCreateLobby] Client {} ({}) creating lobby",
        client.id, client.username);

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
    std::println("[Server::handleCreateLobby] Completed for lobby {}",
        lobby_id);
}

void Server::handleGetAllPublicLobbies(const std::vector<uint8_t>& data,
    const net::Address& sender) {
    sendLobbiesList(sender);
}

void Server::handleAdminStartGame(const std::vector<uint8_t>& data,
    const net::Address& sender) {
    std::println("[Server] handleAdminStartGame: Request from {}:{}",
        sender.getIP(), sender.getPort());

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
    std::println("[Server] handleAdminStartGame: Client {} is in lobby {}",
        client.id, lobby_id);

    if (!isAdmin(sender, lobby_id)) {
        std::println("[Server] handleAdminStartGame: User is not admin");
        sendNotAdmin(sender);
        return;
    }

    std::println("[Server] handleAdminStartGame: Starting game for lobby {}",
        lobby_id);
    sendGameStarting(lobby_id);
    std::println("[Server] handleAdminStartGame: sendGameStarting completed");

    // TODO(Pierre): Initialize game state in lobby
    // std::lock_guard<std::mutex> lock(lobbies_mutex);
    // lobbies.at(lobby_id).getLobby().startGame();
}

void Server::handleLeaveLobby(const std::vector<uint8_t>& data,
    const net::Address& sender) {
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

void Server::handleToggleLobbyVisibility(const std::vector<uint8_t>& data,
    const net::Address& sender) {
    std::println("[Server] handleToggleLobbyVisibility: Request from {}:{}",
        sender.getIP(), sender.getPort());

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

void Server::handleAdminPauseGame(
    const std::vector<uint8_t>& data, const net::Address& sender) {
    std::println("[Server] handleAdminPauseGame: Request from {}:{}",
        sender.getIP(), sender.getPort());

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

    std::println("[Server] handleAdminPauseGame: Toggling pause for lobby {}",
        lobby_id);
    sendAdminGamePaused(lobby_id);
}
