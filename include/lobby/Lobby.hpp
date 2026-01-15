/*
** EPITECH PROJECT, 2025
** TekLeague
** File description:
** Lobby.hpp
*/

#pragma once

    #include <string>
    #include <chrono>
    #include <vector>

    #include <Network/generated_messages.hpp>

    #include "Game.hpp"

enum class LobbyGameState {
    PRE_GAME,   // Lobby en attente de joueurs / configuration
    IN_GAME,    // Partie en cours
    END_GAME    // Partie terminée
};

class Lobby : public Game {
 public:
    Lobby(uint max_players, const std::string& code, const std::string& ppath);

    std::chrono::_V2::system_clock::time_point game_start_time =
        std::chrono::system_clock::now();

    net::TeamScore teams[2];

    std::vector<net::ScoreboardEntry> scoreboard;

    void run() override;

    const std::string& getCode();

    void setCode(const std::string& code);

    const uint getMaxPlayers();

    ////// Game State //////

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

 private:
    uint max_players;
    std::string code;

    // Game state
    LobbyGameState _game_state = LobbyGameState::PRE_GAME;
    bool _lobby_is_public = true;

    // Players in lobby
    std::vector<net::PlayerListEntry> _players_in_lobby;
};
