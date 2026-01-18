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
    #include <vector>

    #include <Network/generated_messages.hpp>
    #include <Network/Address.hpp>
    #include <clock.hpp>

    #include "Game.hpp"
    #include "latencies.hpp"

class LobbyContext {
 public:
    static constexpr std::string PLUGINS_PATH = "server/plugins";

    static constexpr float NASH_SPAWN_DELAY = (60.0f * 1.0f);
    static constexpr float NASH_STAYS_DELAY = (60.0f * 0.5f);

 public:
    LobbyContext(uint32_t max_players, const std::string& code);

    void run();
    const std::string& getCode();

    Game& getLobby();

    void spawnNash();

    void checkNash();

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
    void createPlayersEntities();
    void createOtherEntities();

    /**
     * @brief Get the entity associated with a player's client ID
     * @param client_id The client ID of the player
     * @return std::optional containing the entity if found, std::nullopt otherwise
     */
    ECS::Entity getPlayerEntity(uint32_t client_id) const;

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
     * @brief Try to get entities created if the timestamp delay has passed
     * @return std::optional containing the message if ready, std::nullopt otherwise
     */
    std::optional<net::ENTITIES_CREATED> tryGetEntitiesCreated();

    /**
     * @brief Try to get entities destroyed if the timestamp delay has passed
     * @return std::optional containing the message if ready, std::nullopt otherwise
     */
    std::optional<net::ENTITIES_DESTROYED> tryGetEntitiesDestroyed();

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
     * @brief Try to get game end if triggered
     * @return std::optional containing the message if ready, std::nullopt otherwise
     */
    std::optional<net::GAME_END> tryGetGameEnd();

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
     * @brief Set how often to send entities created (in seconds)
     */
    void setEntitiesCreatedFrequency(float freq) {
        entities_created.delay = size_t(SEC_TO_MICRO(freq));
    }

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

    /**
    * @brief Force get data without checking timestamp 
    * due to packet loss signaled by client
    * @param code The code of the missing packet
    * @return true
    */
    std::vector<uint8_t> forceGetData(uint8_t code);

 private:
    Game lobby;

    ////// Network //////
    std::unordered_map<uint32_t, net::Address> connected_players;
    uint32_t max_clients;
    std::unordered_map<uint32_t, ECS::Entity> _player_entities;

    std::size_t _nashE;
    bool _nashAlive = false;
    te::Timestamp spawn_nash_delay = te::Timestamp(NASH_SPAWN_DELAY);
    te::Timestamp stay_nash_delay = te::Timestamp(NASH_STAYS_DELAY);

    te::Timestamp players_update =
        te::Timestamp(PLAYERS_UPDATES_DEFAULT_LATENCY);
    te::Timestamp buildings_update =
        te::Timestamp(BUILDINGS_UPDATES_DEFAULT_LATENCY);
    te::Timestamp entities_created =
        te::Timestamp(ENTITIES_CREATED_DEFAULT_LATENCY);
    te::Timestamp entities_destroyed =
        te::Timestamp(ENTITIES_DESTROYED_DEFAULT_LATENCY);
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
        * @brief Build the ENTITIES_CREATED message from the lobby's registry
        * @return The constructed message
        */
    net::ENTITIES_CREATED getEntitiesCreated();

    /**
        * @brief Build the ENTITIES_DESTROYED message from the lobby's registry
        * @return The constructed message
        */
    net::ENTITIES_DESTROYED getEntitiesDestroyed();

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
     * @brief Build the GAME_END message from the lobby's state
     * @return The constructed message
     */
    net::GAME_END getGameEnd();

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
