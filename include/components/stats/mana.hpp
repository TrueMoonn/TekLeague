/*
** EPITECH PROJECT, 2025
** GameTwo
** File description:
** mana.hpp
*/

#pragma once

    #include "Game.hpp"

struct Mana {
    Mana(int amount, int regen, int max) :
        amount(amount), regen(regen), max(max) {};

    int amount;
    int regen;
    int max;
};

void registerMana(Game& game);
