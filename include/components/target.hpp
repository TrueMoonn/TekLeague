/*
** EPITECH PROJECT, 2025
** GameTwo
** File description:
** target.hpp
*/

#pragma once

    #include <maths/Vector.hpp>

    #include "physic/components/position.hpp"

    #include "Game.hpp"

struct Target : addon::physic::Position2 {};

void registerTarget(Game& game);
