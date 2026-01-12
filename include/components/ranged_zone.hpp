/*
** EPITECH PROJECT, 2025
** TekLeague
** File description:
** magic_attack.hpp
*/

#pragma once

    #include "Game.hpp"
    #include "clock.hpp"
#include "maths/Vector.hpp"
#include <cstddef>

struct RangedZone {
    RangedZone(int radius) : radius(radius), delay(2.f) {}
    int radius;
    te::Timestamp delay;
};

void registerRangedZone(Game &game);
