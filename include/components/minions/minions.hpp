/*
** EPITECH PROJECT, 2025
** GameTwo
** File description:
** minions.hpp
*/

#pragma once

#include "Game.hpp"

enum class MinionState {
    IDLE = 0,
    MOVEMENT = 1,
    AUTO_ATTACKS = 6,
    SPELL1 = 11,
    SPELL2 = 16,
    DEATH = 21,
};

struct Minions {
    Minions(std::string name) : name(name) {};
    std::string name;
    MinionState state = MinionState::IDLE;
};

void registerMinions(Game& game);
