/*
** EPITECH PROJECT, 2025
** GameTwo
** File description:
** systems.hpp
*/

#pragma once

    #include <functional>
    #include <vector>

    #include "Game.hpp"

void championMovement(Game& game);
void entityDirection(Game& game);
void targetPlayer(Game& game);
void trackTarget(Game& game);

static const std::vector<std::function<void(Game&)>> LOCAL_SYSTEMS = {
    &championMovement,
    &entityDirection,
    &targetPlayer,
    &trackTarget,
};
