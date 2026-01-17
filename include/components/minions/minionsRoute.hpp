/*
** EPITECH PROJECT, 2025
** GameTwo
** File description:
** minionsRoute.hpp
*/

#include "Game.hpp"
#include "maths/Vector.hpp"
#include <queue>

struct MinionsRoute {
    MinionsRoute(std::queue<mat::Vector2i> Positions) : Positions(Positions) {};
    std::queue<mat::Vector2i> Positions;
};

void registerMinionRoute(Game& game);
