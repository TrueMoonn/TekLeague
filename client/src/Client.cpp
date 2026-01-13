/*
** EPITECH PROJECT, 2025
** TekLeague
** File description:
** Game.cpp
*/

#include "configs/entities.hpp"
#include "Client.hpp"

Client::Client() : Game("client/plugins") {
    for (auto& conf : CLIENT_CONFIG_PATHS)
        addConfig(conf);
}

void Client::run() {
    while (_running) {
        if (_framelimit.checkDelay())
            runSystems();
    }
}
