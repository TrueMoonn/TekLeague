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


class Lobby : public Game {
 public:
    static constexpr std::string PLUGINS_PATH = "server/plugins";
 public:
    Lobby(uint max_players, const std::string& code);

    std::chrono::_V2::system_clock::time_point game_start_time =
        std::chrono::system_clock::now();

    net::TeamScore teams[2];

    std::vector<net::ScoreboardEntry> scoreboard;

    void run() override;

    const std::string& getCode();

    const uint getMaxPlayers();

 private:
    uint max_players;
    std::string code;
};
