/*
** EPITECH PROJECT, 2025
** TekLeague
** File description:
** Client.cpp
*/

#include <print>
#include <sys/socket.h>
#include <thread>

#include "Network/generated_messages.hpp"
#include "configs/entities.hpp"
#include "client_systems.hpp"
#include "Client.hpp"

Client::Client()
    : Game(CLIENT_PLUGINS_PATH)
    , te::network::GameClient(COM_DEFAULT_MODE, PROTOCOL_PATH) {

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
    disconnect();
}

void Client::connectToServer(const std::string& ip, uint16_t port) {
    std::print("[+] Connecting to {}:{}...\n", ip, port);

    if (!te::network::GameClient::connectToServer(ip, port)) {
        throw std::runtime_error("Failed to connect to server");
    }

    _server_address = net::Address(ip, port);

    std::print("[Y] Connected to server!\n\n");
}

void Client::disconnect() {
    net::DISCONNEXION msg;
    sendToServer(msg.serialize());
    std::println("[Client] Disconnecting from server...");
    if (isConnected()) {
        disconnectClient();
    }
    _running = false;
}

void Client::receiveMessages() {
    update(0);
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

        handleLoggedIn(msg);
        emit("lobby:logged_in");
    });

    registerPacketHandler(23, [this](const std::vector<uint8_t>& data) {
        std::println("[Client] Received LOGGED_OUT packet");
        auto msg = net::LOGGED_OUT::deserialize(data);

        handleLoggedOut(msg);
        emit("lobby:logged_out");
    });

    registerPacketHandler(25, [this](const std::vector<uint8_t>& data) {
        std::println("[Client] Received USERNAME_ALREADY_TAKEN packet");
        auto msg = net::USERNAME_ALREADY_TAKEN::deserialize(data);

        handleUsernameAlreadyTaken(msg);
        emit("lobby:username_taken");
    });

    registerPacketHandler(31, [this](const std::vector<uint8_t>& data) {
        std::println("[Client] Received LOBBY_JOINED packet");
        auto msg = net::LOBBY_JOINED::deserialize(data);

        handleLobbyJoined(msg);
        emit("lobby:joined", getCode());
    });

    registerPacketHandler(33, [this](const std::vector<uint8_t>& data) {
        std::println("[Client] Received LOBBY_CREATED packet");
        auto msg = net::LOBBY_CREATED::deserialize(data);
        std::string code(msg.lobby_code, 6);
        std::println("[Client] Lobby code: {}", code);

        handleLobbyCreated(msg);
        emit("lobby:created", getCode(), isAdmin());
    });

    registerPacketHandler(35, [this](const std::vector<uint8_t>& data) {
        std::println("[Client] Received LOBBIES_LIST packet");
        auto msg = net::LOBBIES_LIST::deserialize(data);
        std::println("[Client] Number of lobbies: {}", msg.lobby_codes.size());

        handleLobbiesList(msg);
        emit("lobby:lobbies_list");
    });

    registerPacketHandler(37, [this](const std::vector<uint8_t>& data) {
        std::println("[Client] Received GAME_STARTING packet");
        auto msg = net::GAME_STARTING::deserialize(data);

        handleGameStarting(msg);
        emit("game:game_starting");
    });

    registerPacketHandler(38, [this](const std::vector<uint8_t>& data) {
        std::println("[Client] Received PLAYERS_LIST packet");
        auto msg = net::PLAYERS_LIST::deserialize(data);
        std::println("[Client] Number of players: {}", msg.players.size());

        handlePlayersList(msg);
        emit("lobby:players_updated");
    });

    registerPacketHandler(39, [this](const std::vector<uint8_t>& data) {
        std::println("[Client] Received LOBBY_VISIBILITY_CHANGED packet");
        auto msg = net::LOBBY_VISIBILITY_CHANGED::deserialize(data);

        handleLobbyVisibilityChanged(msg);
        emit("lobby:visibility_changed", isPublic());
    });

    registerPacketHandler(42, [this](const std::vector<uint8_t>& data) {
        std::println("[Client] Received LOBBY_DESTROYED packet");
        auto msg = net::LOBBY_DESTROYED::deserialize(data);

        handleLobbyDestroyed(msg);
        emit("lobby:destroyed");
    });

    registerPacketHandler(43, [this](const std::vector<uint8_t>& data) {
        std::println("[Client] Received LOBBY_FULL packet");
        auto msg = net::LOBBY_FULL::deserialize(data);

        handleLobbyFull(msg);
        emit("lobby:full");
    });

    registerPacketHandler(44, [this](const std::vector<uint8_t>& data) {
        std::println("[Client] Received BAD_LOBBY_CODE packet");
        auto msg = net::BAD_LOBBY_CODE::deserialize(data);

        handleBadLobbyCode(msg);
        emit("lobby:bad_code");
    });

    registerPacketHandler(45, [this](const std::vector<uint8_t>& data) {
        std::println("[Client] Received NOT_ADMIN packet");
        auto msg = net::NOT_ADMIN::deserialize(data);

        handleNotAdmin(msg);
        emit("lobby:not_admin");
    });

    registerPacketHandler(47, [this](const std::vector<uint8_t>& data) {
        std::println("[Client] Received TEAM_FULL packet");
        auto msg = net::TEAM_FULL::deserialize(data);

        handleTeamFull(msg);
        emit("lobby:team_full");
    });

    registerPacketHandler(51, [this](const std::vector<uint8_t>& data) {
        std::println("[Client] Received PLAYER_INIT packet");
        auto msg = net::PLAYERS_INIT::deserialize(data);

        handlePlayersInit(msg);
        emit("game:init_player");
    });

    registerPacketHandler(52, [this](const std::vector<uint8_t>& data) {
        std::println("[Client] Received BUILDINGS_INIT packet");
        auto msg = net::BUILDINGS_INIT::deserialize(data);
        handleBuildingsInit(msg);
    });

    registerPacketHandler(53, [this](const std::vector<uint8_t>& data) {
        std::println("[Client] Received ENTITIES_CREATED packet");
        auto msg = net::ENTITIES_CREATED::deserialize(data);

        handleEntitiesCreated(msg);
    });

    registerPacketHandler(54, [this](const std::vector<uint8_t>& data) {
        std::println("[Client] Received ENTITIES_DESTROYED packet");
        auto msg = net::ENTITIES_DESTROYED::deserialize(data);

        handleEntitiesDestroyed(msg);
    });

    registerPacketHandler(61, [this](const std::vector<uint8_t>& data) {
        std::println("[Client] Received PLAYERS_UPDATES packet");
        auto msg = net::PLAYERS_UPDATES::deserialize(data);
        handlePlayersUpdate(msg);
    });

    registerPacketHandler(62, [this](const std::vector<uint8_t>& data) {
        std::println("[Client] Received BUILDINGS_UPDATES packet");
        auto msg = net::BUILDINGS_UPDATES::deserialize(data);
        handleBuildingsUpdate(msg);
    });

    registerPacketHandler(64, [this](const std::vector<uint8_t>& data) {
        std::println("[Client] Received PROJECTILES_UPDATES packet");
        auto msg = net::PROJECTILES_UPDATES::deserialize(data);
        handleProjectilesUpdate(msg);
    });

    registerPacketHandler(88, [this](const std::vector<uint8_t>& data) {
        std::println("[Client] Received ADMIN_GAME_PAUSED packet");
        auto msg = net::ADMIN_GAME_PAUSED::deserialize(data);

        handleAdminGamePaused(msg);
        emit("lobby:game_paused");
    });
}

std::optional<net::PlayerListEntry> Client::getMyInfos() {
    auto& players = getPlayers();
    for (auto& player : players) {
        if (player.id == getClientId())
            return player;
    }
    return std::nullopt;
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
