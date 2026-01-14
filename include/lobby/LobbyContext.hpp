/*
** EPITECH PROJECT, 2025
** TekLeague
** File description:
** LobbyContext.hpp
*/


#pragma once

    #include <optional>

    #include <Network/generated_messages.hpp>
    #include <Network/Address.hpp>

    #include "clock.hpp"
    #include "Lobby.hpp"

enum class LobbyGameState {
    PRE_GAME,   // Lobby en attente de joueurs / configuration
    IN_GAME,    // Partie en cours
    END_GAME    // Partie terminée
};

class LobbyContext {
 public:
    ////// Timestamp default latency in seconds //////
    static constexpr float PLAYERS_UPDATES_DEFAULT_LATENCY = 1.0f / 60.0f;

 public:
    LobbyContext(uint max_players, const std::string& code);

    void run();
    const std::string& getCode();

    Lobby& getLobby();

    ////// Game State Management //////

    /**
     * @brief Get current game state
     */
    LobbyGameState getGameState() const { return game_state; }

    /**
     * @brief Set the game state
     */
    void setGameState(LobbyGameState state) { game_state = state; }

    /**
     * @brief Get current game state as string
     */
    const char* getGameStateString() const {
        switch (game_state) {
            case LobbyGameState::PRE_GAME: return "PRE_GAME";
            case LobbyGameState::IN_GAME: return "IN_GAME";
            case LobbyGameState::END_GAME: return "END_GAME";
            default: return "UNKNOWN";
        }
    }

    /**
     * @brief Check if lobby is in pre-game state
     */
    bool isPreGame() const { return game_state == LobbyGameState::PRE_GAME; }

    /**
     * @brief Check if lobby is in game
     */
    bool isInGame() const { return game_state == LobbyGameState::IN_GAME; }

    /**
     * @brief Check if lobby game has ended
     */
    bool isEndGame() const { return game_state == LobbyGameState::END_GAME; }

    ////// Network //////

    bool addClient(const net::Address& address, uint32_t client_id);
    void removeClient(uint32_t client_id);
    const std::unordered_map<uint32_t, net::Address>& getClients() const;
    bool isFull() const;

    /**
     * @brief Try to get player updates if the timestamp delay has passed
     * @return std::optional containing the message if ready, std::nullopt otherwise
     */
    std::optional<net::PLAYERS_UPDATES> tryGetPlayerUpdates();


    ////// setters //////

    /**
     * @brief Set how often to send player updates (in seconds)
     */
    void setPlayersUpdateFrequency(float freq) {
        players_update.delay = size_t(SEC_TO_MICRO(freq));
    }

 private:
    Lobby lobby;

    ////// Game State //////
    LobbyGameState game_state = LobbyGameState::PRE_GAME;

    ////// Network //////
    std::unordered_map<uint32_t, net::Address> connected_players;
    uint max_clients;

    te::Timestamp players_update = te::Timestamp(PLAYERS_UPDATES_DEFAULT_LATENCY);

    /**
     * @brief Build the PLAYERS_UPDATES message from the lobby's registry
     * @return The constructed message
     */
    net::PLAYERS_UPDATES getPlayerUpdates();

    /**
     * @brief Build the BUILDING_UPDATES message from the lobby's registry
     * @return The constructed message
     */
    net::BUILDINGS_UPDATES getBuildingsUpdates();

    /**
     * @brief Build the CREATURES_UPDATES message from the lobby's registry
     * @return The constructed message
     */
    net::CREATURES_UPDATES getCreaturesUpdates();

    /**
     * @brief Build the PROJECTILES_UPDATES message from the lobby's registry
     * @return The constructed message
     */
    net::PROJECTILES_UPDATES getProjectilesUpdates();

    /**
     * @brief Build the COLLECTIBLES_UPDATES message from the lobby's registry
     * @return The constructed message
     */
    net::COLLECTIBLES_UPDATES getCollectiblesUpdates();
};
