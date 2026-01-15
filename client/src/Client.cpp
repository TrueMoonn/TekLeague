/*
** EPITECH PROJECT, 2025
** TekLeague
** File description:
** Client.cpp
*/

#include <print>
#include <chrono>
#include <sys/socket.h>
#include <thread>

#include "Network/ProtocolManager.hpp"
#include "Network/generated_messages.hpp"
#include "configs/entities.hpp"
#include "client_systems.hpp"
#include "Client.hpp"

Client::Client() :
    Game(CLIENT_PLUGINS_PATH)
    , te::network::GameClient(COM_DEFAULT_MODE, PROTOCOL_PATH)
    , _lobby_data(*this) {
    for (auto& conf : CLIENT_CONFIG_PATHS)
        addConfig(conf);
    for (auto& sys : CLIENT_SYSTEMS)
        sys(*this);

    createSystem("poll_event");
    createSystem("display");
    createSystem("draw");
    createSystem("draw_text");
    createSystem("animate");

    registerMessageHandlers();
}

Client::~Client() {
    std::println("[Client] Cleaning up resources...");
    if (isConnected()) {
        disconnectClient();
    }
}

void Client::connectToServer(const std::string& ip, uint16_t port) {
    std::print("[+] Connecting to {}:{}...\n", ip, port);

    if (!te::network::GameClient::connectToServer(ip, port)) {
        throw std::runtime_error("Failed to connect to server");
    }

    net::Address server_addr(ip, port);
    _lobby_data.setServerAddress(server_addr);

    std::print("[Y] Connected to server!\n\n");
}

void Client::disconnect() {
    std::println("[Client] Disconnecting from server...");
    if (isConnected()) {
        disconnectClient();
    }
    _running = false;
}

void Client::receiveMessages() {
    update(0);
}

void Client::updateGame() {
    if (_framelimit.checkDelay()) {
        runSystems();
    }
}

void Client::handlePing() {
    sendPong();
}

void Client::handlePong() {
    // TODO(PIERRE): si ca fait longtemps = deco
}

void Client::sendPing() {
    net::PING msg;
    sendToServer(msg.serialize());
}

void Client::sendPong() {
    net::PONG msg;
    sendToServer(msg.serialize());
}

void Client::registerMessageHandlers() {
    registerPacketHandler(6, [this](const std::vector<uint8_t>& data) {
        (void)data;
        handlePing();
    });
    registerPacketHandler(7, [this](const std::vector<uint8_t>& data) {
        (void)data;
        handlePong();
    });
    registerPacketHandler(22, [this](const std::vector<uint8_t>& data) {
        std::println("[Client] Received LOGGED_IN packet ({} bytes)", data.size());
        auto msg = net::LOGGED_IN::deserialize(data);
        std::println("[Client] Client ID: {}", msg.id);

        _lobby_data.handleLoggedIn(msg);

        emit("lobby:logged_in");
    });

    registerPacketHandler(23, [this](const std::vector<uint8_t>& data) {
        std::println("[Client] Received LOGGED_OUT packet");
        auto msg = net::LOGGED_OUT::deserialize(data);

        _lobby_data.handleLoggedOut(msg);

        emit("lobby:logged_out");
    });

    registerPacketHandler(25, [this](const std::vector<uint8_t>& data) {
        std::println("[Client] Received USERNAME_ALREADY_TAKEN packet");
        auto msg = net::USERNAME_ALREADY_TAKEN::deserialize(data);

        _lobby_data.handleUsernameAlreadyTaken(msg);

        emit("lobby:username_taken");
    });

    registerPacketHandler(31, [this](const std::vector<uint8_t>& data) {
        std::println("[Client] Received LOBBY_JOINED packet");
        auto msg = net::LOBBY_JOINED::deserialize(data);

        _lobby_data.handleLobbyJoined(msg);

        emit("lobby:joined", _lobby_data.getLobbyCode());
    });

    registerPacketHandler(33, [this](const std::vector<uint8_t>& data) {
        std::println("[Client] Received LOBBY_CREATED packet");
        auto msg = net::LOBBY_CREATED::deserialize(data);
        std::string code(msg.lobby_code, 6);
        std::println("[Client] Lobby code: {}", code);

        _lobby_data.handleLobbyCreated(msg);

        emit("lobby:created", _lobby_data.getLobbyCode(), _lobby_data.isAdmin());
    });

    registerPacketHandler(35, [this](const std::vector<uint8_t>& data) {
        std::println("[Client] Received LOBBIES_LIST packet");
        auto msg = net::LOBBIES_LIST::deserialize(data);
        std::println("[Client] Number of lobbies: {}", msg.lobby_codes.size());

        _lobby_data.handleLobbiesList(msg);

        emit("lobby:lobbies_list", _lobby_data.getCachedLobbiesList());
    });

    registerPacketHandler(37, [this](const std::vector<uint8_t>& data) {
        std::println("[Client] Received GAME_STARTING packet");
        auto msg = net::GAME_STARTING::deserialize(data);

        _lobby_data.handleGameStarting(msg);

        emit("lobby:game_starting");
    });

    registerPacketHandler(38, [this](const std::vector<uint8_t>& data) {
        std::println("[Client] Received PLAYERS_LIST packet");
        auto msg = net::PLAYERS_LIST::deserialize(data);
        std::println("[Client] Number of players: {}", msg.players.size());

        _lobby_data.handlePlayersList(msg);

        emit("lobby:players_updated", _lobby_data.getPlayersInLobby());
    });

    registerPacketHandler(39, [this](const std::vector<uint8_t>& data) {
        std::println("[Client] Received LOBBY_VISIBILITY_CHANGED packet");
        auto msg = net::LOBBY_VISIBILITY_CHANGED::deserialize(data);

        _lobby_data.handleLobbyVisibilityChanged(msg);

        emit("lobby:visibility_changed", _lobby_data.isLobbyPublic());
    });

    registerPacketHandler(42, [this](const std::vector<uint8_t>& data) {
        std::println("[Client] Received LOBBY_DESTROYED packet");
        auto msg = net::LOBBY_DESTROYED::deserialize(data);

        _lobby_data.handleLobbyDestroyed(msg);

        emit("lobby:destroyed");
    });

    registerPacketHandler(43, [this](const std::vector<uint8_t>& data) {
        std::println("[Client] Received LOBBY_FULL packet");
        auto msg = net::LOBBY_FULL::deserialize(data);

        _lobby_data.handleLobbyFull(msg);

        emit("lobby:full");
    });

    registerPacketHandler(44, [this](const std::vector<uint8_t>& data) {
        std::println("[Client] Received BAD_LOBBY_CODE packet");
        auto msg = net::BAD_LOBBY_CODE::deserialize(data);

        _lobby_data.handleBadLobbyCode(msg);

        emit("lobby:bad_code");
    });

    registerPacketHandler(45, [this](const std::vector<uint8_t>& data) {
        std::println("[Client] Received NOT_ADMIN packet");
        auto msg = net::NOT_ADMIN::deserialize(data);

        _lobby_data.handleNotAdmin(msg);

        emit("lobby:not_admin");
    });

    registerPacketHandler(47, [this](const std::vector<uint8_t>& data) {
        std::println("[Client] Received TEAM_FULL packet");
        auto msg = net::TEAM_FULL::deserialize(data);

        _lobby_data.handleTeamFull(msg);

        emit("lobby:team_full");
    });

    registerPacketHandler(88, [this](const std::vector<uint8_t>& data) {
        std::println("[Client] Received ADMIN_GAME_PAUSED packet");
        auto msg = net::ADMIN_GAME_PAUSED::deserialize(data);

        _lobby_data.handleAdminGamePaused(msg);

        emit("lobby:game_paused");
    });

    registerPacketHandler(61, [this](const std::vector<uint8_t>& data) {
        std::println("[Client] Received PLAYERS_UPDATES packet");
        // TODO(Pierre): Handle PLAYERS_UPDATES when in game
        auto msg = net::PLAYERS_UPDATES::deserialize(data);
        // Update game state with player positions, hp, etc.
    });
}

void Client::run() {
    std::println("[Client] Starting main game loop...");
    while (_running) {
        receiveMessages();

        if (_framelimit.checkDelay())
            runSystems();

        // Small sleep to avoid burning CPU
        std::this_thread::sleep_for(std::chrono::milliseconds(16));
    }
    std::println("[Client] Main game loop ended");
}
