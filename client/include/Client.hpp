/*
** EPITECH PROJECT, 2025
** TekLeague
** File description:
** Client.hpp
*/

#pragma once

#include <optional>

#include <maths/Vector.hpp>
#include <network1/GameClient.hpp>

#include "Game.hpp"
#include "Network/generated_messages.hpp"
#include "clock.hpp"

class Client : public Game, public te::network::GameClient {
 public:
    static constexpr const char* CLIENT_PLUGINS_PATH = "client/plugins";
    static constexpr const char* PROTOCOL_PATH = "config/protocol.json";
    static constexpr const char* COM_DEFAULT_MODE = "UDP";

 public:
    Client();
    ~Client();

    void run() override;

    /**
     * @brief Connect to server
     */
    void connectToServer(const std::string& ip, uint16_t port);

    /**
     * @brief Disconnect from server
     */
    void disconnect();

    /**
     * @brief Check if client is running
     */
    bool isRunning() const { return _running; }

    /**
     * @brief Receive messages from server
     */
    void receiveMessages();

    ////// Network senders //////
    void sendLogin(const std::string& username);
    void sendLogout();
    void sendCreateLobby();
    void sendJoinLobby(const std::string& code);
    void sendGetPublicLobbies();
    void sendLeaveLobby();
    void sendStartGame();
    void sendToggleVisibility();
    void sendPauseGame();
    void sendWantThisTeam(uint8_t team);
    void sendPacketLoss(uint8_t code);

    ////// Getters //////
    const std::string& getUsername() const { return _username; }
    void setUsername(const std::string& username) { _username = username; }
    uint32_t getClientId() const { return _client_id; }
    bool isAdmin() const { return _is_admin; }
    const std::vector<std::string>& getCachedLobbiesList() const {
        return _cached_lobbies_list; }
    bool isLoggedIn() const { return !_username.empty(); }
    bool isInLobby() const { return !getCode().empty(); }
    std::optional<net::PlayerListEntry> getMyInfos();

    mat::Vector2i mpos;
    te::Timestamp inputLimit{0.08f};
 private:
    std::string _username;
    uint32_t _client_id = 0;
    bool _is_admin = false;
    std::vector<std::string> _cached_lobbies_list;
    net::Address _server_address;

    void registerPacketTrackers();

    ////// Message handlers //////
    void registerMessageHandlers();

    void handlePing();
    void handlePong();
    void sendPing();
    void sendPong();
    void handleLoggedIn(const net::LOGGED_IN& msg);
    void handleLoggedOut(const net::LOGGED_OUT& msg);
    void handleUsernameAlreadyTaken(const net::USERNAME_ALREADY_TAKEN& msg);
    void handleLobbyCreated(const net::LOBBY_CREATED& msg);
    void handleLobbyJoined(const net::LOBBY_JOINED& msg);
    void handleBadLobbyCode(const net::BAD_LOBBY_CODE& msg);
    void handleLobbyFull(const net::LOBBY_FULL& msg);
    void handlePlayersList(const net::PLAYERS_LIST& msg);
    void handleLobbiesList(const net::LOBBIES_LIST& msg);
    void handleLobbyVisibilityChanged(const net::LOBBY_VISIBILITY_CHANGED& msg);
    void handleGameStarting(const net::GAME_STARTING& msg);
    void handleLobbyDestroyed(const net::LOBBY_DESTROYED& msg);
    void handleNotAdmin(const net::NOT_ADMIN& msg);
    void handleAdminGamePaused(const net::ADMIN_GAME_PAUSED& msg);
    void handleTeamFull(const net::TEAM_FULL& msg);
    void handlePlayersInit(const net::PLAYERS_INIT& msg);
    void handleBuildingsInit(const net::BUILDINGS_INIT& msg);
    void handleEntitiesCreated(const net::ENTITIES_CREATED& msg);
    void handleEntitiesDestroyed(const net::ENTITIES_DESTROYED& msg);
    void handlePlayersUpdate(const net::PLAYERS_UPDATES& msg);
    void handleBuildingsUpdate(const net::BUILDINGS_UPDATES& msg);
    void handleProjectilesUpdate(const net::PROJECTILES_UPDATES& msg);
};
