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

    #include <clock.hpp>
    #include <network1/GameServer.hpp>
    #include <Network/ProtocolManager.hpp>
    #include <Network/Address.hpp>
    #include "LobbyContext.hpp"

struct PlayerInfo {
    uint32_t id;
    std::string username;
    uint32_t lobby_id = 0;  // 0 means not in a lobby
    bool in_lobby = false;
    uint8_t team = 0;  // 0 = no team, 1 = team 1, 2 = team 2
    net::Address address;
};

class Server : public te::network::GameServer {
 public:
    explicit Server(uint16_t port = 6767, const std::string& protocol = "UDP");

    void run();

    /**
     * @brief Request server to stop (signal-safe)
     * Only sets the stop flag, doesn't join threads
     */
    void requestStop() { _should_run.store(false); }

    /**
     * @brief Stop the server gracefully
     * Performs cleanup and joins threads
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
    te::Timestamp ping{5.0f};
    std::unordered_map<net::Address, PlayerInfo> clients;

    // username -> address
    std::unordered_map<std::string, net::Address> usernames;
    // lobby_id -> admin address
    std::unordered_map<uint32_t, net::Address> lobby_admins;
    uint32_t next_client_id = 1;

    uint32_t generateClientId();
    std::optional<std::reference_wrapper
        <PlayerInfo>> getClient(const net::Address& address);
    std::optional<std::reference_wrapper
        <PlayerInfo>> getClientById(uint32_t client_id);
    bool isUsernameAvailable(const std::string& username);
    bool isAdmin(const net::Address& address, uint32_t lobby_id);

    ////// Lobbies //////
    std::unordered_map<uint32_t, LobbyContext> lobbies;
    std::unordered_map<std::string, uint32_t> lobby_codes;
    std::unordered_set<uint32_t> public_lobbies;
    uint32_t next_lobby_id = 1;
    std::mutex lobbies_mutex;  // Protects lobbies, lobby_codes, public_lobbies

    ////// Server Control //////
    std::atomic<bool> _should_run{true};

     te::Timestamp lobbies_list_timestamp{2.0f};

    uint32_t createLobby(uint32_t max_clients, const net::Address& admin);
    void destroyLobby(uint32_t lobby_id);
    // NOTE: This function assumes lobbies_mutex is already locked!
    void destroyLobbyUnsafe(uint32_t lobby_id);
    void broadcastToLobby(uint32_t lobby_id, const std::vector<uint8_t>& data);
    void sendToLobby(uint32_t lobby_id,
        const std::vector<uint8_t>& data, const net::Address& exclude);

    // NOTE: This function assumes lobbies_mutex is already locked!
    void broadcastToLobbyUnsafe(uint32_t lobby_id,
        const std::vector<uint8_t>& data);

    /**
     * @brief Generate a unique 6-character alphanumeric lobby code
     * @return A unique lobby code that doesn't exist yet
     */
    std::string generateUniqueLobbyCode();

    ////// Handlers //////
    void handleConnectionRequest(const std::vector<uint8_t>& data,
        const net::Address& sender);
    void handlePing(const std::vector<uint8_t>& data,
        const net::Address& sender);
    void handlePong(const std::vector<uint8_t>& data,
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
    void handleClientInput(const std::vector<uint8_t>& data,
        const net::Address& sender);
    void handleAdminPauseGame(const std::vector<uint8_t>& data,
        const net::Address& sender);
    void handleAdminEndGame(const std::vector<uint8_t>& data,
        const net::Address& sender);
    void handleDisconnection(const std::vector<uint8_t>& data,
        const net::Address& sender);
    void handlePacketLoss(const std::vector<uint8_t>& data,
        const net::Address& sender);

    ////// Senders //////
    void sendPing(const net::Address& address, uint32_t client_id);
    void sendPong(const net::Address& address, uint32_t client_id);
    void sendLoggedIn(const net::Address& address, uint32_t client_id);
    void sendLoggedOut(const net::Address& address, uint32_t client_id);
    void sendUsernameAlreadyTaken(const net::Address& address);
    void sendLobbyJoined(const net::Address& address, uint32_t client_id);
    void sendLobbyCreated(const net::Address& address,
        const std::string& lobby_code);
    // NOTE: Assumes lobbies_mutex is already locked!
    void sendLobbiesListUnsafe();
    void sendLobbiesList(const net::Address& address);
    void sendGameStarting(uint32_t lobby_id);
    void sendPlayersList(uint32_t lobby_id);
      // NOTE: Assumes lobbies_mutex is already locked!
    void sendPlayersListUnsafe(uint32_t lobby_id);
    void sendLobbyVisibilityChanged(uint32_t lobby_id, bool is_public);
    void sendLobbyDestroyed(uint32_t lobby_id);
    void sendLobbyFull(const net::Address& address);
    void sendBadLobbyCode(const net::Address& address);
    void sendNotAdmin(const net::Address& address);
    void sendTeamFull(const net::Address& address);
    void sendPlayersNotInTeam(const net::Address& address);
    void sendAdminGamePaused(uint32_t lobby_id);
    void sendGameEnded(uint32_t lobby_id);
    void sendPlayersInit(uint32_t lobby_id);
    void sendBuildingsInit(uint32_t lobby_id);

    ////// Lobby State Queries //////

    /**
     * @brief Get the game state of a specific lobby
     * @param lobby_id The lobby ID to query
     * @return Optional LobbyGameState if lobby exists
     */
    std::optional<LobbyGameState> getLobbyGameState(uint32_t lobby_id);

    /**
     * @brief Check if a lobby is in pre-game state
     */
    bool isLobbyPreGame(uint32_t lobby_id);

    /**
     * @brief Check if a lobby is in game
     */
    bool isLobbyInGame(uint32_t lobby_id);

    /**
     * @brief Check if a lobby game has ended
     */
    bool isLobbyEndGame(uint32_t lobby_id);
};
