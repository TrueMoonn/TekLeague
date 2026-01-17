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

#include <network1/GameServer.hpp>

#include "Network/generated_messages.hpp"
#include "Server.hpp"

bool Server::setPacketsHandlers() {
    registerPacketHandler(2, [this](const std::vector<uint8_t>& data,
        const net::Address& sender) {
        handleDisconnection(data, sender);
    });
    registerPacketHandler(6, [this](const std::vector<uint8_t>& data,
        const net::Address& sender) {
        handlePing(data, sender);
    });
    registerPacketHandler(7, [this](const std::vector<uint8_t>& data,
        const net::Address& sender) {
        handlePong(data, sender);
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
    registerPacketHandler(41, [this](const std::vector<uint8_t>& data,
        const net::Address& sender) {
        handleWantThisTeam(data, sender);
    });
    registerPacketHandler(46, [this](const std::vector<uint8_t>& data,
        const net::Address& sender) {
        handleToggleLobbyVisibility(data, sender);
    });
    registerPacketHandler(87, [this](const std::vector<uint8_t>& data,
        const net::Address& sender) {
        handleAdminPauseGame(data, sender);
    });
    registerPacketHandler(89, [this](const std::vector<uint8_t>& data,
        const net::Address& sender) {
        handleAdminEndGame(data, sender);
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
    std::println("[Server::handleDisconnection] Client removed from clients map");
}

void Server::handlePing(const std::vector<uint8_t>& data,
    const net::Address& sender) {
    net::PONG msg;
    sendTo(sender, msg.serialize());
}

void Server::handlePong(const std::vector<uint8_t>& data,
    const net::Address& sender) {
    // TODO(PIERRE): mettre à jour le délai du joueur
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

    uint32_t lobby_id;
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

    uint32_t lobby_id = createLobby(10, sender);

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

    uint32_t lobby_id = client.lobby_id;
    std::println("[Server] handleAdminStartGame: Client {} is in lobby {}",
        client.id, lobby_id);

    if (!isAdmin(sender, lobby_id)) {
        std::println("[Server] handleAdminStartGame: User is not admin");
        sendNotAdmin(sender);
        return;
    }

    std::println("[Server] handleAdminStartGame: Starting game for lobby {}",
        lobby_id);

    // Change lobby state to IN_GAME
    {
        std::lock_guard<std::mutex> lock(lobbies_mutex);
        if (lobbies.find(lobby_id) != lobbies.end()) {
            lobbies.at(lobby_id).setGameState(LobbyGameState::IN_GAME);
            std::println("[Server] handleAdminStartGame: Lobby {} state changed to IN_GAME",
                lobby_id);
        }
    }

    sendGameStarting(lobby_id);
    std::println("[Server] handleAdminStartGame: sendGameStarting completed");

    // TODO(Pierre): Initialize game state in lobby
    // lobbies.at(lobby_id).getLobby().startGame();
    // sendPlayersInit(lobby_id);
    // sendBuildingsInit(lobby_id);
}

void Server::handleLeaveLobby(const std::vector<uint8_t>& data,
    const net::Address& sender) {
    auto client_opt = getClient(sender);
    if (!client_opt)
        return;

    auto& client = client_opt->get();
    if (!client.in_lobby)
        return;

    uint32_t lobby_id = client.lobby_id;

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

        if (lobby_is_empty) {
            std::println("[Server] handleLeaveLobby: Lobby {} is now empty, cleaning up",
                lobby_id);
            std::string code = lobby_ctx.getCode();
            lobby_codes.erase(code);
            lobby_admins.erase(lobby_id);
            lobbies.erase(lobby_id);
            public_lobbies.erase(lobby_id);
        }
    }

    client.in_lobby = false;
    client.lobby_id = 0;

    if (lobby_is_empty) {
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

    uint32_t lobby_id = client.lobby_id;

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

void Server::handleWantThisTeam(const std::vector<uint8_t>& data,
    const net::Address& sender) {
    std::println("[Server] handleWantThisTeam: Request from {}:{}",
        sender.getIP(), sender.getPort());

    auto client_opt = getClient(sender);
    if (!client_opt) {
        std::println("[Server] handleWantThisTeam: Client not found");
        return;
    }

    auto& client = client_opt->get();
    if (!client.in_lobby) {
        std::println("[Server] handleWantThisTeam: Client not in lobby");
        return;
    }

    net::WANT_THIS_TEAM msg = net::WANT_THIS_TEAM::deserialize(data);
    uint8_t requested_team = msg.team;

    std::println("[Server] handleWantThisTeam: Client {} wants team {}",
        client.id, static_cast<int>(requested_team));

    if (requested_team != 1 && requested_team != 2) {
        std::println("[Server] handleWantThisTeam: Invalid team number");
        return;
    }

    uint32_t lobby_id = client.lobby_id;

    int team_count = 0;
    {
        std::lock_guard<std::mutex> lock(lobbies_mutex);
        for (const auto& [other_id, other_addr] : lobbies.at(lobby_id).getClients()) {
            auto other_client_opt = getClient(other_addr);
            if (other_client_opt) {
                auto& other_client = other_client_opt->get();
                if (other_client.team == requested_team) {
                    team_count++;
                }
            }
        }
    }

    if (team_count >= 3) {
        std::println("[Server] handleWantThisTeam: Team {} is full",
            static_cast<int>(requested_team));
        sendTeamFull(sender);
        return;
    }

    client.team = requested_team;
    std::println("[Server] handleWantThisTeam: Client {} assigned to team {}",
        client.id, static_cast<int>(requested_team));

    sendPlayersList(lobby_id);
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

    uint32_t lobby_id = client.lobby_id;

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

void Server::handleAdminEndGame(const std::vector<uint8_t>& data,
    const net::Address& sender) {
    std::println("[Server] handleAdminEndGame: Request from {}:{}",
        sender.getIP(), sender.getPort());

    auto client_opt = getClient(sender);
    if (!client_opt) {
        std::println("[Server] handleAdminEndGame: Client not found");
        return;
    }

    auto& client = client_opt->get();
    if (!client.in_lobby) {
        std::println("[Server] handleAdminEndGame: Client not in lobby");
        return;
    }

    uint32_t lobby_id = client.lobby_id;
    std::println("[Server] handleAdminEndGame: Client {} is in lobby {}",
        client.id, lobby_id);

    if (!isAdmin(sender, lobby_id)) {
        std::println("[Server] handleAdminEndGame: User is not admin");
        sendNotAdmin(sender);
        return;
    }

    // Change lobby state to END_GAME
    {
        std::lock_guard<std::mutex> lock(lobbies_mutex);
        if (lobbies.find(lobby_id) != lobbies.end()) {
            lobbies.at(lobby_id).setGameState(LobbyGameState::END_GAME);
            std::println("[Server] handleAdminEndGame: Lobby {} state changed to END_GAME",
                lobby_id);
        }
    }

    std::println("[Server] handleAdminEndGame: Ending game for lobby {}",
        lobby_id);
    sendGameEnded(lobby_id);
    std::println("[Server] handleAdminEndGame: sendGameEnded completed");
}
