/*
** EPITECH PROJECT, 2025
** GameTwo
** File description:
** client_systems.hpp
*/

#pragma once

    #include <vector>
    #include <functional>

    #include "Client.hpp"

void championMovement(Game&);
void buttonStates(Client&);
void updateHud(Client&);

static const std::vector<std::function<void(Client&)>> CLIENT_SYSTEMS = {
    &buttonStates,
    &championMovement,
    &updateHud,
};
