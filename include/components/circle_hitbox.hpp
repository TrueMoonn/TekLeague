/*
** EPITECH PROJECT, 2026
** TekLeague
** File description:
** circle_hitbox
*/

#pragma once

    #include "Game.hpp"

struct CircleHitbox {
    CircleHitbox(float radius, bool pusback)
        : radius(radius), pusback(pusback) {}
    float radius;
    bool pusback;
};

void registerCircleHitbox(Game &game);
