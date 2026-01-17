/*
** EPITECH PROJECT, 2025
** GameTwo
** File description:
** health_bar.hpp
*/

#pragma once

#include "ECS/Entity.hpp"
#include "Game.hpp"
#include "entities.hpp"

struct Sticky {
    Sticky(int x, int y) : x(x), y(y) {}
    int x;
    int y;
    ECS::Entity id = (ECS::Entity)eType::CHAMPION + 1;
};

void registerSticky(Game& game);
