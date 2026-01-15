/*
** EPITECH PROJECT, 2025
** GameTwo
** File description:
** team.hpp
*/

#pragma once

#include <GameTool.hpp>

struct Health {
    Health(int hp, int hp_max, int hp_regen) :
    amount(hp), max(hp_max), regen(hp_regen) {}

    int amount;
    int max;
    int regen;
};

void registerHealth(te::GameTool& game);
