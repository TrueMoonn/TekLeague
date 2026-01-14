/*
** EPITECH PROJECT, 2025
** TekLeague
** File description:
** Server.hpp
*/

#pragma once

    #include <cstdint>
    #include <string>
    #include <unordered_map>
    #include <unordered_set>
    #include <vector>
    #include <optional>
    #include <functional>
    #include <mutex>
    #include <atomic>

    #include <network/GameServer.hpp>
    #include <Network/ProtocolManager.hpp>
    #include <Network/Address.hpp>
    #include "lobby/LobbyContext.hpp"

struct PlayerInfo {
    uint32_t id;
    std::string username;
    uint lobby_id = 0;  // 0 means not in a lobby
    bool in_lobby = false;
    uint8_t team = 0;  // 0 = no team, 1 = team 1, 2 = team 2
    net::Address address;
};

class Server : public te::network::GameServer {
 public:
    explicit Server(uint16_t port = 6767, const std::string& protocol = "UDP");

    void run();

    /**
     * @brief Stop the server gracefully
     */
    void stop();

    /**
     * @brief Check if server should continue running
     */
    bool shouldRun() const { return _should_run; }

    void sendAutomatic();

    /**
     * @brief Set how often to send PLAYERS_UPDATES (in seconds)
     */
    void setPlayersUpdateFrequency(std::size_t lobby_id, float frequency) {
        lobbies.at(lobby_id).setPlayersUpdateFrequency(frequency);
    }

 private:
    bool setPacketsHandlers();

    ////// Client Management //////
    std::unordered_map<net::Address, PlayerInfo> clients;

    // username -> address
    std::unordered_map<std::string, net::Address> usernames;
    // lobby_id -> admin address
    std::unordered_map<uint, net::Address> lobby_admins;
    uint32_t next_client_id = 1;

    uint32_t generateClientId();
    std::optional<std::reference_wrapper
        <PlayerInfo>> getClient(const net::Address& address);
    std::optional<std::reference_wrapper
        <PlayerInfo>> getClientById(uint32_t client_id);
    bool isUsernameAvailable(const std::string& username);
    bool isAdmin(const net::Address& address, uint lobby_id);

    ////// Lobbies //////
    std::unordered_map<uint, LobbyContext> lobbies;
    std::unordered_map<std::string, uint> lobby_codes;
    std::unordered_set<uint> public_lobbies;
    uint next_lobby_id = 1;
    std::mutex lobbies_mutex;  // Protects lobbies, lobby_codes, public_lobbies

    ////// Server Control //////
    std::atomic<bool> _should_run{true};

    uint createLobby(uint max_clients, const net::Address& admin);
    void destroyLobby(uint lobby_id);
    void broadcastToLobby(uint lobby_id, const std::vector<uint8_t>& data);
    void sendToLobby(uint lobby_id,
        const std::vector<uint8_t>& data, const net::Address& exclude);

    // NOTE: This function assumes lobbies_mutex is already locked!
    void broadcastToLobbyUnsafe(uint lobby_id,
        const std::vector<uint8_t>& data);

    /**
     * @brief Generate a unique 6-character alphanumeric lobby code
     * @return A unique lobby code that doesn't exist yet
     */
    std::string generateUniqueLobbyCode();

    ////// Handlers //////
    void handleConnectionRequest(const std::vector<uint8_t>& data,
        const net::Address& sender);
    void handleLogin(const std::vector<uint8_t>& data,
        const net::Address& sender);
    void handleLogout(const std::vector<uint8_t>& data,
        const net::Address& sender);
    void handleJoinLobby(const std::vector<uint8_t>& data,
        const net::Address& sender);
    void handleCreateLobby(const std::vector<uint8_t>& data,
        const net::Address& sender);
    void handleGetAllPublicLobbies(const std::vector<uint8_t>& data,
        const net::Address& sender);
    void handleAdminStartGame(const std::vector<uint8_t>& data,
        const net::Address& sender);
    void handleLeaveLobby(const std::vector<uint8_t>& data,
        const net::Address& sender);
    void handleToggleLobbyVisibility(const std::vector<uint8_t>& data,
        const net::Address& sender);
    void handleWantThisTeam(const std::vector<uint8_t>& data,
        const net::Address& sender);
    void handleAdminPauseGame(const std::vector<uint8_t>& data,
        const net::Address& sender);
    void handleAdminEndGame(const std::vector<uint8_t>& data,
        const net::Address& sender);
    void handleDisconnection(const std::vector<uint8_t>& data,
        const net::Address& sender);

    ////// Senders //////
    void sendPlayersUpdate();
    void sendLoggedIn(const net::Address& address, uint32_t client_id);
    void sendLoggedOut(const net::Address& address, uint32_t client_id);
    void sendUsernameAlreadyTaken(const net::Address& address);
    void sendLobbyJoined(const net::Address& address, uint32_t client_id);
    void sendLobbyCreated(const net::Address& address,
        const std::string& lobby_code);
    void sendLobbiesList(const net::Address& address);
    void sendGameStarting(uint lobby_id);
    void sendPlayersList(uint lobby_id);
    void sendLobbyVisibilityChanged(uint lobby_id, bool is_public);
    void sendLobbyDestroyed(uint lobby_id);
    void sendLobbyFull(const net::Address& address);
    void sendBadLobbyCode(const net::Address& address);
    void sendNotAdmin(const net::Address& address);
    void sendTeamFull(const net::Address& address);
    void sendAdminGamePaused(uint lobby_id);
    void sendGameEnded(uint lobby_id);
    
    ////// Lobby State Queries //////
    
    /**
     * @brief Get the game state of a specific lobby
     * @param lobby_id The lobby ID to query
     * @return Optional LobbyGameState if lobby exists
     */
    std::optional<LobbyGameState> getLobbyGameState(uint lobby_id);
    
    /**
     * @brief Check if a lobby is in pre-game state
     */
    bool isLobbyPreGame(uint lobby_id);
    
    /**
     * @brief Check if a lobby is in game
     */
    bool isLobbyInGame(uint lobby_id);
    
    /**
     * @brief Check if a lobby game has ended
     */
    bool isLobbyEndGame(uint lobby_id);
};
