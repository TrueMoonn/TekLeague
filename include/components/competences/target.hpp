/*
** EPITECH PROJECT, 2025
** GameTwo
** File description:
** target.hpp
*/

#pragma once

    #include <maths/Vector.hpp>

    #include "Game.hpp"

struct Target : mat::Vector2f {
    ECS::Entity to_attack = 0;
};

void registerTarget(Game& game);
