/*
** EPITECH PROJECT, 2025
** TekLeague
** File description:
** LobbyManager.hpp
*/

#pragma once

#include <string>
#include <cstdint>

#include <Network/Address.hpp>
#include <Network/generated_messages.hpp>
#include <network/GameClient.hpp>

/**
 * @brief Manages lobby interaction on the client side
 * Handles CLI-based lobby creation, joining, and management
 */
class LobbyManager {
 public:
    enum class State {
        DISCONNECTED,           // Not connected to server
        LOGGING_IN,             // Waiting for login response
        LOBBY_MENU,             // Main menu: create/join/list lobbies
        CREATING_LOBBY,         // Waiting for lobby creation response
        JOINING_LOBBY,          // Waiting for join response
        FETCHING_LOBBIES,       // Waiting for public lobbies list
        IN_LOBBY,               // Inside a lobby, waiting for game start
        WAITING_GAME_START,     // Waiting for game start response
        WAITING_VISIBILITY,     // Waiting for visibility change response
        WAITING_PAUSE,          // Waiting for pause response
        IN_GAME                 // Game is running
    };

    explicit LobbyManager(te::network::GameClient& network_client);

    /**
     * @brief Set the server address
     */
    void setServerAddress(const net::Address& address);

    /**
     * @brief Main update loop - handles CLI input and state
     */
    void update();

    /**
     * @brief Get current state
     */
    State getState() const { return _state; }

    /**
     * @brief Check if in game
     */
    bool isInGame() const { return _state == State::IN_GAME; }

    // Message handlers (called when receiving messages from server)
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

    /**
     * @brief Get current username
     */
    const std::string& getUsername() const { return _username; }

    /**
     * @brief Get current lobby code
     */
    const std::string& getLobbyCode() const { return _lobby_code; }

    /**
     * @brief Get client ID
     */
    uint32_t getClientId() const { return _client_id; }

    /**
     * @brief Check if client is admin
     */
    bool isAdmin() const { return _is_admin; }

 private:
    // State management
    State _state = State::DISCONNECTED;
    te::network::GameClient& _network_client;
    net::Address _server_address;

    // Client info
    std::string _username;
    uint32_t _client_id = 0;
    std::string _lobby_code;
    bool _is_admin = false;
    bool _lobby_is_public = true;

    // Players in current lobby
    std::vector<net::PlayerListEntry> _players_in_lobby;

    // CLI interaction
    void showMainMenu();
    void showLobbyMenu();
    void promptLogin();
    void promptCreateOrJoin();
    void promptJoinLobby();
    void promptLobbyCode();

    // Send functions
    void sendLogin(const std::string& username);
    void sendLogout();
    void sendCreateLobby();
    void sendJoinLobby(const std::string& code);
    void sendGetPublicLobbies();
    void sendLeaveLobby();
    void sendStartGame();
    void sendToggleVisibility();
    void sendPauseGame();

    // Utility
    void changeState(State new_state);
    void displayPlayers();
    std::string readLine();
    void clearScreen();
};
