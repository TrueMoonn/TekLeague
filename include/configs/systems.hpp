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
void autoAttacks(Game& game);
void trackTarget(Game& game);
void spawnDeathEntity(Game &game);
void handleTimeout(Game &game);
void killAllEntity(Game &game);
void dealDamage(Game &game);
void circlePushback(Game &game);


static const std::vector<std::function<void(Game&)>> LOCAL_SYSTEMS = {
    &championMovement,
    &entityDirection,
    &autoAttacks,
    &trackTarget,
    &spawnDeathEntity,
    &handleTimeout,
    &killAllEntity,
    &dealDamage,
    &circlePushback
};
