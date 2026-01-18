/*
** EPITECH PROJECT, 2025
** GameTwo
** File description:
** mana.hpp
*/

#pragma once

    #include "Game.hpp"

struct Mana {
    Mana(int amount, int max_amount, int regen) :
        amount(amount), max_amount(max_amount), regen(regen) {};

    int amount;
    int max_amount;
    int regen;
};

void registerMana(Game& game);
