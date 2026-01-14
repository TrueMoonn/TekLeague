/*
** EPITECH PROJECT, 2025
** TekLeague
** File description:
** Client.cpp
*/

#include <iostream>
#include <chrono>
#include <thread>

#include "configs/entities.hpp"
#include "client_systems.hpp"
#include "Client.hpp"

Client::Client() :
    Game(CLIENT_PLUGINS_PATH)
    , te::network::GameClient(COM_DEFAULT_MODE, PROTOCOL_PATH)
    , _lobby_manager(*this) {
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
    std::cout << "[Client] Cleaning up resources..." << std::endl;
    if (isConnected()) {
        disconnectClient();
    }
}

void Client::connectToServer(const std::string& ip, uint16_t port) {
    std::cout << "[+] Connecting to " << ip << ":" << port << "...\n";

    if (!te::network::GameClient::connectToServer(ip, port)) {
        throw std::runtime_error("Failed to connect to server");
    }

    net::Address server_addr(ip, port);
    _lobby_manager.setServerAddress(server_addr);

    std::cout << "[Y] Connected to server!\n\n";
}

void Client::disconnect() {
    std::cout << "[Client] Disconnecting from server..." << std::endl;
    if (isConnected()) {
        disconnectClient();
    }
    _running = false;
}

void Client::receiveMessages() {
    update(0);
}

void Client::updateLobby() {
    _lobby_manager.update();
}

void Client::updateGame() {
    if (_framelimit.checkDelay()) {
        runSystems();
    }
}

void Client::registerMessageHandlers() {
    registerPacketHandler(22, [this](const std::vector<uint8_t>& data) {
        std::cout << "[Client] Received LOGGED_IN packet (" << data.size() << " bytes)" << std::endl;
        auto msg = net::LOGGED_IN::deserialize(data);
        std::cout << "[Client] Client ID: " << msg.id << std::endl;
        _lobby_manager.handleLoggedIn(msg);
    });

    registerPacketHandler(23, [this](const std::vector<uint8_t>& data) {
        std::cout << "[Client] Received LOGGED_OUT packet" << std::endl;
        auto msg = net::LOGGED_OUT::deserialize(data);
        _lobby_manager.handleLoggedOut(msg);
    });

    registerPacketHandler(25, [this](const std::vector<uint8_t>& data) {
        std::cout << "[Client] Received USERNAME_ALREADY_TAKEN packet" << std::endl;
        auto msg = net::USERNAME_ALREADY_TAKEN::deserialize(data);
        _lobby_manager.handleUsernameAlreadyTaken(msg);
    });

    registerPacketHandler(31, [this](const std::vector<uint8_t>& data) {
        std::cout << "[Client] Received LOBBY_JOINED packet" << std::endl;
        auto msg = net::LOBBY_JOINED::deserialize(data);
        _lobby_manager.handleLobbyJoined(msg);
    });

    registerPacketHandler(33, [this](const std::vector<uint8_t>& data) {
        std::cout << "[Client] Received LOBBY_CREATED packet" << std::endl;
        auto msg = net::LOBBY_CREATED::deserialize(data);
        std::string code(msg.lobby_code, 6);
        std::cout << "[Client] Lobby code: " << code << std::endl;
        _lobby_manager.handleLobbyCreated(msg);
    });

    registerPacketHandler(35, [this](const std::vector<uint8_t>& data) {
        std::cout << "[Client] Received LOBBIES_LIST packet" << std::endl;
        auto msg = net::LOBBIES_LIST::deserialize(data);
        std::cout << "[Client] Number of lobbies: " << msg.lobby_codes.size() << std::endl;
        _lobby_manager.handleLobbiesList(msg);
    });

    registerPacketHandler(37, [this](const std::vector<uint8_t>& data) {
        std::cout << "[Client] Received GAME_STARTING packet" << std::endl;
        auto msg = net::GAME_STARTING::deserialize(data);
        _lobby_manager.handleGameStarting(msg);
    });

    registerPacketHandler(38, [this](const std::vector<uint8_t>& data) {
        std::cout << "[Client] Received PLAYERS_LIST packet" << std::endl;
        auto msg = net::PLAYERS_LIST::deserialize(data);
        std::cout << "[Client] Number of players: " << msg.players.size() << std::endl;
        _lobby_manager.handlePlayersList(msg);
    });

    registerPacketHandler(39, [this](const std::vector<uint8_t>& data) {
        std::cout << "[Client] Received LOBBY_VISIBILITY_CHANGED packet" << std::endl;
        auto msg = net::LOBBY_VISIBILITY_CHANGED::deserialize(data);
        _lobby_manager.handleLobbyVisibilityChanged(msg);
    });

    registerPacketHandler(42, [this](const std::vector<uint8_t>& data) {
        std::cout << "[Client] Received LOBBY_DESTROYED packet" << std::endl;
        auto msg = net::LOBBY_DESTROYED::deserialize(data);
        _lobby_manager.handleLobbyDestroyed(msg);
    });

    registerPacketHandler(43, [this](const std::vector<uint8_t>& data) {
        std::cout << "[Client] Received LOBBY_FULL packet" << std::endl;
        auto msg = net::LOBBY_FULL::deserialize(data);
        _lobby_manager.handleLobbyFull(msg);
    });

    registerPacketHandler(44, [this](const std::vector<uint8_t>& data) {
        std::cout << "[Client] Received BAD_LOBBY_CODE packet" << std::endl;
        auto msg = net::BAD_LOBBY_CODE::deserialize(data);
        _lobby_manager.handleBadLobbyCode(msg);
    });

    registerPacketHandler(45, [this](const std::vector<uint8_t>& data) {
        std::cout << "[Client] Received NOT_ADMIN packet" << std::endl;
        auto msg = net::NOT_ADMIN::deserialize(data);
        _lobby_manager.handleNotAdmin(msg);
    });

    registerPacketHandler(88, [this](const std::vector<uint8_t>& data) {
        std::cout << "[Client] Received ADMIN_GAME_PAUSED packet" << std::endl;
        auto msg = net::ADMIN_GAME_PAUSED::deserialize(data);
        _lobby_manager.handleAdminGamePaused(msg);
    });

    registerPacketHandler(61, [this](const std::vector<uint8_t>& data) {
        std::cout << "[Client] Received PLAYERS_UPDATES packet" << std::endl;
        // TODO(Pierre): Handle PLAYERS_UPDATES when in game
        auto msg = net::PLAYERS_UPDATES::deserialize(data);
        // Update game state with player positions, hp, etc.
    });
}

void Client::run() {
    std::cout << "[Client] Starting main game loop..." << std::endl;
    while (_running) {
        receiveMessages();

        if (_framelimit.checkDelay())
            runSystems();

        if (!_lobby_manager.isInGame()) {
            updateLobby();
        } else {
            // Run game logic
            updateGame();
        }

        // Small sleep to avoid burning CPU
        std::this_thread::sleep_for(std::chrono::milliseconds(16));
    }
    std::cout << "[Client] Main game loop ended" << std::endl;
}
