/*
** EPITECH PROJECT, 2025
** GameTwo
** File description:
** Game.hpp
*/

#pragma once

    #include <unordered_map>
    #include <string>
    #include <chrono>
    #include <vector>

    #include <ECS/Entity.hpp>
    #include <GameTool.hpp>
    #include <clock.hpp>
    #include <Network/generated_messages.hpp>

    #include "entities.hpp"

    #define FRAME_LIMIT 1.0f / 60   // 60 fps

enum class LobbyGameState {
    PRE_GAME,   // Lobby en attente de joueurs / configuration
    IN_GAME,    // Partie en cours
    END_GAME    // Partie terminée
};

class Game : public te::GameTool {
 public:
    Game(const std::string& ppath);
    Game(uint max_players, const std::string& code, const std::string& ppath);

    ECS::Entity nextEntity(eType type);

    void AddKillEntity(ECS::Entity e) {
        this->_EntityToKill.push_back(e);
    };
    std::vector<ECS::Entity> getAllEntityToKill() {
        return this->_EntityToKill;
    };

    void popEntity(ECS::Entity e) {
        this->_EntityToKill.erase(std::find(this->_EntityToKill.begin(),
            this->_EntityToKill.end(), e));
    };

    virtual void run();

    ////// Lobby Data //////
    std::chrono::_V2::system_clock::time_point game_start_time =
        std::chrono::system_clock::now();

    net::TeamScore teams[2];

    std::vector<net::ScoreboardEntry> scoreboard;

    ////// Lobby Management //////
    const std::string& getCode() const;
    void setCode(const std::string& code);
    const uint getMaxPlayers() const;

    ////// Game State Management //////

    /**
     * @brief Get current game state
     */
    LobbyGameState getGameState() const { return _game_state; }

    /**
     * @brief Set the game state
     */
    void setGameState(LobbyGameState state) { _game_state = state; }

    /**
     * @brief Check if lobby is in pre-game state
     */
    bool isPreGame() const { return _game_state == LobbyGameState::PRE_GAME; }

    /**
     * @brief Check if game is in progress
     */
    bool isInGame() const { return _game_state == LobbyGameState::IN_GAME; }

    /**
     * @brief Check if lobby game has ended
     */
    bool isEndGame() const { return _game_state == LobbyGameState::END_GAME; }

    /**
     * @brief Check if lobby is public
     */
    bool isPublic() const { return _lobby_is_public; }

    /**
     * @brief Set lobby visibility
     */
    void setPublic(bool is_public) { _lobby_is_public = is_public; }

    ////// Players Management //////

    /**
     * @brief Get players in lobby
     */
    const std::vector<net::PlayerListEntry>& getPlayers() const { return _players_in_lobby; }

    /**
     * @brief Set players list
     */
    void setPlayers(const std::vector<net::PlayerListEntry>& players) { _players_in_lobby = players; }

    /**
     * @brief Clear players list
     */
    void clearPlayers() { _players_in_lobby.clear(); }

 protected:
    bool _running;
    te::Timestamp _framelimit;
 private:
    std::unordered_map<eType, ECS::Entity> _nextEntities;
    std::vector<ECS::Entity> _EntityToKill;

    ////// Lobby Data //////
    uint _max_players = 0;
    std::string _code;

    // Game state
    LobbyGameState _game_state = LobbyGameState::PRE_GAME;
    bool _lobby_is_public = true;

    std::vector<net::PlayerListEntry> _players_in_lobby;
};
