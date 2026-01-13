/*
** EPITECH PROJECT, 2025
** GameTwo
** File description:
** xp.hpp
*/

#pragma once

    #include "Game.hpp"

struct Xp {
    Xp(std::size_t am) : amount(am) {};
    std::size_t amount;
};

void registerXp(Game& game);
