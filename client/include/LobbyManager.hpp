/*
** EPITECH PROJECT, 2025
** TekLeague
** File description:
** LobbyDataManager.hpp
*/

#pragma once

#include <string>
#include <cstdint>
#include <vector>

#include <Network/Address.hpp>
#include <Network/generated_messages.hpp>
#include <network/GameClient.hpp>

/**
 * @brief Manages lobby data and network communication on the client side
 * Provides data storage and network helpers for lobby operations
 * Does NOT handle UI - signals are emitted by the Client for UI updates
 */
class LobbyDataManager {
 public:
    explicit LobbyDataManager(te::network::GameClient& network_client);

    /**
     * @brief Set the server address
     */
    void setServerAddress(const net::Address& address);

    ////// Network //////

    /**
     * @brief Send login request to server
     */
    void sendLogin(const std::string& username);

    /**
     * @brief Send logout request
     */
    void sendLogout();

    /**
     * @brief Send create lobby request
     */
    void sendCreateLobby();

    /**
     * @brief Send join lobby request
     */
    void sendJoinLobby(const std::string& code);

    /**
     * @brief Send request for public lobbies list
     */
    void sendGetPublicLobbies();

    /**
     * @brief Send leave lobby request
     */
    void sendLeaveLobby();

    /**
     * @brief Send start game request (admin only)
     */
    void sendStartGame();

    /**
     * @brief Send toggle visibility request (admin only)
     */
    void sendToggleVisibility();

    /**
     * @brief Send pause game request (admin only)
     */
    void sendPauseGame();

    ////// MESSAGE HANDLERS (update data only) //////

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

    ////// GETTERS //////

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

    /**
     * @brief Check if lobby is public
     */
    bool isLobbyPublic() const { return _lobby_is_public; }

    /**
     * @brief Get players in current lobby
     */
    const std::vector<net::PlayerListEntry>& getPlayersInLobby() const { return _players_in_lobby; }

    /**
     * @brief Get cached list of public lobbies
     */
    const std::vector<std::string>& getCachedLobbiesList() const { return _cached_lobbies_list; }

    /**
     * @brief Check if logged in
     */
    bool isLoggedIn() const { return !_username.empty(); }

    /**
     * @brief Check if in lobby
     */
    bool isInLobby() const { return !_lobby_code.empty(); }

    /**
     * @brief Check if in game
     */
    bool isInGame() const { return _in_game; }

 private:
    te::network::GameClient& _network_client;
    net::Address _server_address;

    // Client info
    std::string _username;
    uint32_t _client_id = 0;

    // Lobby info
    std::string _lobby_code;
    bool _is_admin = false;
    bool _lobby_is_public = true;
    bool _in_game = false;

    // Players in current lobby
    std::vector<net::PlayerListEntry> _players_in_lobby;

    // Cached list of public lobbies
    std::vector<std::string> _cached_lobbies_list;
};
