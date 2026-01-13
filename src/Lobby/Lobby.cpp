/*
** EPITECH PROJECT, 2025
** TekLeague
** File description:
** Game.cpp
*/

#include "lobby/Lobby.hpp"

Lobby::Lobby(uint max_clients, const std::string& code) :
    Game(PLUGINS_PATH), max_players(max_clients), code(code) {
    // gérer les scenes ici ?
}

void Lobby::run() {
    if (_framelimit.checkDelay())
        runSystems();
}

const std::string& Lobby::getCode() {
    return code;
}

const uint Lobby::getMaxPlayers() {
    return max_players;
}
