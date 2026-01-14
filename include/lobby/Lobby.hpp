/*
** EPITECH PROJECT, 2025
** TekLeague
** File description:
** Lobby.hpp
*/

#pragma once

    #include <string>

    #include "Game.hpp"

class Lobby : public Game {
 public:
    static constexpr std::string PLUGINS_PATH = "server/plugins";
 public:
    Lobby(uint max_players, const std::string& code);

    void run() override;

    const std::string& getCode();

    const uint getMaxPlayers();

 private:
    uint max_players;
    std::string code;
};
