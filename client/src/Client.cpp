/*
** EPITECH PROJECT, 2025
** TekLeague
** File description:
** Game.cpp
*/

#include "configs/entities.hpp"
#include "client_systems.hpp"

#include "Client.hpp"

Client::Client() : Game("client/plugins") {
    for (auto& conf : CLIENT_CONFIG_PATHS)
        addConfig(conf);
    for (auto& sys : CLIENT_SYSTEMS)
        sys(*this);
}
