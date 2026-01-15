/*
** EPITECH PROJECT, 2025
** TekLeague
** File description:
** Game.cpp
*/

#include "lobby/Lobby.hpp"

Lobby::Lobby(uint max_clients, const std::string& code,
    const std::string& ppath) :
    Game(ppath), max_players(max_clients), code(code) {
}

void Lobby::run() {
    if (_framelimit.checkDelay())
        runSystems();
}

const std::string& Lobby::getCode() {
    return code;
}

void Lobby::setCode(const std::string& new_code) {
    code = new_code;
}

const uint Lobby::getMaxPlayers() {
    return max_players;
}
