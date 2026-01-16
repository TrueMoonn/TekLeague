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

void autoAttacks(Game& game);
void trackTarget(Game& game);
void killAllEntity(Game &game);
void dealDamage(Game &game);
void targetedDamage(Game &game);

static const std::vector<std::function<void(Game&)>> LOCAL_SYSTEMS = {
    &autoAttacks,
    &trackTarget,
    &killAllEntity,
    &dealDamage,
    &targetedDamage,
};
