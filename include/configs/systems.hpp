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

void autoAttacks(Game&);
void trackTarget(Game&);
void killAllEntity(Game&);
void dealDamage(Game&);
void targetedDamage(Game&);
void minionsMovement(Game&);
void spawnDeathEntity(Game&);
void handleTimeout(Game&);
void circlePushback(Game&);
void regeneration(Game&);
void fountainInteractions(Game&);

static const std::vector<std::function<void(Game&)>> LOCAL_SYSTEMS = {
    &autoAttacks,
    &trackTarget,
    &dealDamage,
    &targetedDamage,
    &minionsMovement,
    &spawnDeathEntity,
    &handleTimeout,
    &circlePushback,
    &killAllEntity,
    &regeneration,
    &fountainInteractions,
};
