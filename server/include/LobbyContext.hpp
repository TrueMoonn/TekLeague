/*
** EPITECH PROJECT, 2025
** TekLeague
** File description:
** LobbyContext.hpp
*/


#pragma once

    #include <optional>
    #include <string>
    #include <unordered_map>

    #include <Network/generated_messages.hpp>
    #include <Network/Address.hpp>
    #include <clock.hpp>

    #include "lobby/Lobby.hpp"

class LobbyContext {
 public:
    ////// Timestamp default latency in seconds //////
    static constexpr float PLAYERS_UPDATES_DEFAULT_LATENCY = 1.0f / 120.0f;
    static constexpr float BUILDINGS_UPDATES_DEFAULT_LATENCY = 1.0f / 10.0f;
    static constexpr float CREATURES_UPDATES_DEFAULT_LATENCY = 1.0f / 60.0f;
    static constexpr float PROJECTILES_UPDATES_DEFAULT_LATENCY = 1.0f / 60.0f;
    static constexpr float COLLECTIBLES_UPDATES_DEFAULT_LATENCY = 1.0f / 10.0f;
    static constexpr float INVENTORIES_UPDATES_DEFAULT_LATENCY = 1.0f / 10.0f;
    static constexpr float STATS_UPDATES_DEFAULT_LATENCY = 1.0f / 10.0f;

    static constexpr float SCORE_DEFAULT_LATENCY = 1.0f / 1.0f;
    static constexpr float GAME_DURATION_DEFAULT_LATENCY = 1.0f / 1.0f;
    static constexpr float SCOREBOARD_DEFAULT_LATENCY = 1.0f / 1.0f;

    static constexpr float PLAYERS_LIST_DEFAULT_LATENCY = 0.5f;

    static constexpr std::string PLUGINS_PATH = "server/plugins";

 public:
    LobbyContext(uint max_players, const std::string& code);

    void run();
    const std::string& getCode();

    Lobby& getLobby();

    ////// Game State Management //////

    /**
     * @brief Get current game state
     */
    LobbyGameState getGameState() const { return lobby.getGameState(); }

    /**
     * @brief Set the game state
     */
    void setGameState(LobbyGameState state) { lobby.setGameState(state); }

    /**
     * @brief Get current game state as string
     */
    const char* getGameStateString() const {
        switch (lobby.getGameState()) {
            case LobbyGameState::PRE_GAME: return "PRE_GAME";
            case LobbyGameState::IN_GAME: return "IN_GAME";
            case LobbyGameState::END_GAME: return "END_GAME";
            default: return "UNKNOWN";
        }
    }

    /**
     * @brief Check if lobby is in pre-game state
     */
    bool isPreGame() const { return lobby.isPreGame(); }

    /**
     * @brief Check if lobby is in game
     */
    bool isInGame() const { return lobby.isInGame(); }

    /**
     * @brief Check if lobby game has ended
     */
    bool isEndGame() const { return lobby.isEndGame(); }

    ////// Network //////

    bool addClient(const net::Address& address, uint32_t client_id);
    void removeClient(uint32_t client_id);
    const std::unordered_map<uint32_t, net::Address>& getClients() const;
    bool isFull() const;

    /**
        * @brief Build the PLAYERS_INIT message from the lobby's registry
        * @return The constructed message
        */
    net::PLAYERS_INIT getPlayersInit();

    /**
    * @brief Build the BUILDINGS_INIT message from the lobby's registry
    * @return The constructed message
    */
    net::BUILDINGS_INIT getBuildingsInit();

    /**
     * @brief Try to get player updates if the timestamp delay has passed
     * @return std::optional containing the message if ready, std::nullopt otherwise
     */
    std::optional<net::PLAYERS_UPDATES> tryGetPlayerUpdates();

    /**
     * @brief Try to get buildings updates if the timestamp delay has passed
     * @return std::optional containing the message if ready, std::nullopt otherwise
     */
    std::optional<net::BUILDINGS_UPDATES> tryGetBuildingsUpdates();

    /**
     * @brief Try to get creatures updates if the timestamp delay has passed
     * @return std::optional containing the message if ready, std::nullopt otherwise
     */
    std::optional<net::CREATURES_UPDATES> tryGetCreaturesUpdates();

    /**
     * @brief Try to get projectiles updates if the timestamp delay has passed
     * @return std::optional containing the message if ready, std::nullopt otherwise
     */
    std::optional<net::PROJECTILES_UPDATES> tryGetProjectilesUpdates();

    /**
     * @brief Try to get collectibles updates if the timestamp delay has passed
     * @return std::optional containing the message if ready, std::nullopt otherwise
     */
    std::optional<net::COLLECTIBLES_UPDATES> tryGetCollectiblesUpdates();

    /**
     * @brief Try to get inventories updates if the timestamp delay has passed
     * @return std::optional containing the message if ready, std::nullopt otherwise
     */
    std::optional<net::INVENTORIES_UPDATES> tryGetInventoriesUpdates();

    /**
     * @brief Try to get stats updates if the timestamp delay has passed
     * @return std::optional containing the message if ready, std::nullopt otherwise
     */
    std::optional<net::STATS_UPDATES> tryGetStatsUpdates();

    /**
     * @brief Try to get score if the timestamp delay has passed
     * @return std::optional containing the message if ready, std::nullopt otherwise
     */
    std::optional<net::SCORE> tryGetScore();

    /**
     * @brief Try to get game duration if the timestamp delay has passed
     * @return std::optional containing the message if ready, std::nullopt otherwise
     */
    std::optional<net::GAME_DURATION> tryGetGameDuration();

    /**
     * @brief Try to get scoreboard if the timestamp delay has passed
     * @return std::optional containing the message if ready, std::nullopt otherwise
     */
    std::optional<net::SCOREBOARD> tryGetScoreboard();

    ////// setters //////

    /**
     * @brief Set how often to send players updates (in seconds)
     */
    void setPlayersUpdateFrequency(float freq) {
        players_update.delay = size_t(SEC_TO_MICRO(freq));
    }

    /**
     * @brief Set how often to send buildings updates (in seconds)
     */
    void setBuildingsUpdateFrequency(float freq) {
        buildings_update.delay = size_t(SEC_TO_MICRO(freq));
    }

    /**
     * @brief Set how often to send creatures updates (in seconds)
     */
    void setCreaturesUpdateFrequency(float freq) {
        creatures_update.delay = size_t(SEC_TO_MICRO(freq));
    }

    /**
     * @brief Set how often to send projectiles updates (in seconds)
     */
    void setProjectilesUpdateFrequency(float freq) {
        projectiles_update.delay = size_t(SEC_TO_MICRO(freq));
    }

    /**
     * @brief Set how often to send collectibles updates (in seconds)
     */
    void setCollectiblesUpdateFrequency(float freq) {
        collectibles_update.delay = size_t(SEC_TO_MICRO(freq));
    }

    /**
     * @brief Set how often to send inventories updates (in seconds)
     */
    void setInventoriesUpdateFrequency(float freq) {
        inventories_update.delay = size_t(SEC_TO_MICRO(freq));
    }

    /**
     * @brief Set how often to send stats updates (in seconds)
     */
    void setStatsUpdateFrequency(float freq) {
        stats_update.delay = size_t(SEC_TO_MICRO(freq));
    }

    /**
     * @brief Set how often to send score (in seconds)
     */
    void setScoreFrequency(float freq) {
        score_update.delay = size_t(SEC_TO_MICRO(freq));
    }

    /**
     * @brief Set how often to send game duration (in seconds)
     */
    void setGameDurationFrequency(float freq) {
        game_duration_update.delay = size_t(SEC_TO_MICRO(freq));
    }

    /**
     * @brief Set how often to send scoreboard (in seconds)
     */
    void setScoreboardFrequency(float freq) {
        scoreboard_update.delay = size_t(SEC_TO_MICRO(freq));
    }

    /**
     * @brief Set how often to send players list in PRE_GAME (in seconds)
     */
    void setPlayersListFrequency(float freq) {
        players_list_update.delay = size_t(SEC_TO_MICRO(freq));
    }

    /**
     * @brief Check if players list should be sent (timestamp expired and in PRE_GAME)
     * @return true if players list should be sent, false otherwise
     */
    bool shouldSendPlayersList();

 private:
    Lobby lobby;

    ////// Network //////
    std::unordered_map<uint32_t, net::Address> connected_players;
    uint max_clients;

    te::Timestamp players_update =
        te::Timestamp(PLAYERS_UPDATES_DEFAULT_LATENCY);
    te::Timestamp buildings_update =
        te::Timestamp(BUILDINGS_UPDATES_DEFAULT_LATENCY);
    te::Timestamp creatures_update =
        te::Timestamp(CREATURES_UPDATES_DEFAULT_LATENCY);
    te::Timestamp projectiles_update =
        te::Timestamp(PROJECTILES_UPDATES_DEFAULT_LATENCY);
    te::Timestamp collectibles_update =
        te::Timestamp(COLLECTIBLES_UPDATES_DEFAULT_LATENCY);
    te::Timestamp inventories_update =
        te::Timestamp(INVENTORIES_UPDATES_DEFAULT_LATENCY);
    te::Timestamp stats_update =
        te::Timestamp(STATS_UPDATES_DEFAULT_LATENCY);

    te::Timestamp score_update =
        te::Timestamp(SCORE_DEFAULT_LATENCY);
    te::Timestamp game_duration_update =
        te::Timestamp(GAME_DURATION_DEFAULT_LATENCY);
    te::Timestamp scoreboard_update =
        te::Timestamp(SCOREBOARD_DEFAULT_LATENCY);

    te::Timestamp players_list_update =
        te::Timestamp(PLAYERS_LIST_DEFAULT_LATENCY);

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

    /**
     * @brief Build the INVENTORIES_UPDATES message from the lobby's registry
     * @return The constructed message
     */
    net::INVENTORIES_UPDATES getInventoriesUpdates();

    /**
     * @brief Build the STATS_UPDATES message from the lobby's registry
     * @return The constructed message
     */
    net::STATS_UPDATES getStatsUpdates();

    /**
     * @brief Build the STATS_UPDATES message from the lobby's registry
     * @return The constructed message
     */
    net::SCORE getScore();

    /**
     * @brief Build the STATS_UPDATES message from the lobby's registry
     * @return The constructed message
     */
    net::GAME_DURATION getGameDuration();

    /**
     * @brief Build the STATS_UPDATES message from the lobby's registry
     * @return The constructed message
     */
    net::SCOREBOARD getScoreboard();
};
