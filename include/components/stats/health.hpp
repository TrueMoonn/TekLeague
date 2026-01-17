/*
** EPITECH PROJECT, 2025
** GameTwo
** File description:
** team.hpp
*/

#pragma once

    #include "Game.hpp"

struct Health {
    Health(int hp, int hp_max, int hp_regen) :
    amount(hp), max_amount(hp_max), regen(hp_regen) {}

    void reduceSafely(const std::int64_t &);
    int amount;
    int max_amount;
    int regen;
};

void registerHealth(Game& game);
