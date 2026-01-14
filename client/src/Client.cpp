/*
** EPITECH PROJECT, 2025
** TekLeague
** File description:
** Client.cpp
*/

#include "configs/entities.hpp"
#include "Client.hpp"
#include <iostream>
#include <chrono>
#include <thread>

Client::Client() :
    Game("client/plugins")
    , _network_client("UDP", "config/protocol.json") {
    for (auto& conf : CLIENT_CONFIG_PATHS)
        addConfig(conf);

    registerMessageHandlers();
}

Client::~Client() {
    std::cout << "[Client] Cleaning up resources..." << std::endl;
    if (_network_client.isConnected()) {
        _network_client.disconnect();
    }
}

void Client::connectToServer(const std::string& ip, uint16_t port) {
    std::cout << "[+] Connecting to " << ip << ":" << port << "...\n";

    if (!_network_client.connectToServer(ip, port)) {
        throw std::runtime_error("Failed to connect to server");
    }

    net::Address server_addr(ip, port);

    _lobby_manager.setSendFunction([this](const std::vector<uint8_t>& data) {
        _network_client.sendToServer(data);
    });

    _lobby_manager.setServerAddress(server_addr);

    std::cout << "[ ] Connected to server!\n\n";
}

void Client::disconnect() {
    std::cout << "[Client] Disconnecting from server..." << std::endl;
    if (_network_client.isConnected()) {
        _network_client.disconnect();
    }
    _running = false;
}

void Client::receiveMessages() {
    _network_client.update(0);
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
    _network_client.registerPacketHandler(22, [this](const std::vector<uint8_t>& data) {
        std::cout << "[Client] Received LOGGED_IN packet (" << data.size() << " bytes)" << std::endl;
        auto msg = net::LOGGED_IN::deserialize(data);
        std::cout << "[Client] Client ID: " << msg.id << std::endl;
        _lobby_manager.handleLoggedIn(msg);
    });

    _network_client.registerPacketHandler(23, [this](const std::vector<uint8_t>& data) {
        std::cout << "[Client] Received LOGGED_OUT packet" << std::endl;
        auto msg = net::LOGGED_OUT::deserialize(data);
        _lobby_manager.handleLoggedOut(msg);
    });

    _network_client.registerPacketHandler(25, [this](const std::vector<uint8_t>& data) {
        std::cout << "[Client] Received USERNAME_ALREADY_TAKEN packet" << std::endl;
        auto msg = net::USERNAME_ALREADY_TAKEN::deserialize(data);
        _lobby_manager.handleUsernameAlreadyTaken(msg);
    });

    _network_client.registerPacketHandler(31, [this](const std::vector<uint8_t>& data) {
        std::cout << "[Client] Received LOBBY_JOINED packet" << std::endl;
        auto msg = net::LOBBY_JOINED::deserialize(data);
        _lobby_manager.handleLobbyJoined(msg);
    });

    _network_client.registerPacketHandler(33, [this](const std::vector<uint8_t>& data) {
        std::cout << "[Client] Received LOBBY_CREATED packet" << std::endl;
        auto msg = net::LOBBY_CREATED::deserialize(data);
        std::string code(msg.lobby_code, 6);
        std::cout << "[Client] Lobby code: " << code << std::endl;
        _lobby_manager.handleLobbyCreated(msg);
    });

    _network_client.registerPacketHandler(35, [this](const std::vector<uint8_t>& data) {
        std::cout << "[Client] Received LOBBIES_LIST packet" << std::endl;
        auto msg = net::LOBBIES_LIST::deserialize(data);
        std::cout << "[Client] Number of lobbies: " << msg.lobby_codes.size() << std::endl;
        _lobby_manager.handleLobbiesList(msg);
    });

    _network_client.registerPacketHandler(37, [this](const std::vector<uint8_t>& data) {
        std::cout << "[Client] Received GAME_STARTING packet" << std::endl;
        auto msg = net::GAME_STARTING::deserialize(data);
        _lobby_manager.handleGameStarting(msg);
    });

    _network_client.registerPacketHandler(38, [this](const std::vector<uint8_t>& data) {
        std::cout << "[Client] Received PLAYERS_LIST packet" << std::endl;
        auto msg = net::PLAYERS_LIST::deserialize(data);
        std::cout << "[Client] Number of players: " << msg.players.size() << std::endl;
        _lobby_manager.handlePlayersList(msg);
    });

    _network_client.registerPacketHandler(39, [this](const std::vector<uint8_t>& data) {
        std::cout << "[Client] Received LOBBY_VISIBILITY_CHANGED packet" << std::endl;
        auto msg = net::LOBBY_VISIBILITY_CHANGED::deserialize(data);
        _lobby_manager.handleLobbyVisibilityChanged(msg);
    });

    _network_client.registerPacketHandler(42, [this](const std::vector<uint8_t>& data) {
        std::cout << "[Client] Received LOBBY_DESTROYED packet" << std::endl;
        auto msg = net::LOBBY_DESTROYED::deserialize(data);
        _lobby_manager.handleLobbyDestroyed(msg);
    });

    _network_client.registerPacketHandler(43, [this](const std::vector<uint8_t>& data) {
        std::cout << "[Client] Received LOBBY_FULL packet" << std::endl;
        auto msg = net::LOBBY_FULL::deserialize(data);
        _lobby_manager.handleLobbyFull(msg);
    });

    _network_client.registerPacketHandler(44, [this](const std::vector<uint8_t>& data) {
        std::cout << "[Client] Received BAD_LOBBY_CODE packet" << std::endl;
        auto msg = net::BAD_LOBBY_CODE::deserialize(data);
        _lobby_manager.handleBadLobbyCode(msg);
    });

    _network_client.registerPacketHandler(45, [this](const std::vector<uint8_t>& data) {
        std::cout << "[Client] Received NOT_ADMIN packet" << std::endl;
        auto msg = net::NOT_ADMIN::deserialize(data);
        _lobby_manager.handleNotAdmin(msg);
    });

    _network_client.registerPacketHandler(88, [this](const std::vector<uint8_t>& data) {
        std::cout << "[Client] Received ADMIN_GAME_PAUSED packet" << std::endl;
        auto msg = net::ADMIN_GAME_PAUSED::deserialize(data);
        _lobby_manager.handleAdminGamePaused(msg);
    });

    _network_client.registerPacketHandler(61, [this](const std::vector<uint8_t>& data) {
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
