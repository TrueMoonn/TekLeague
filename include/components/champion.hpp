/*
** EPITECH PROJECT, 2025
** GameTwo
** File description:
** champion.hpp
*/

#pragma once

    #include <array>

    #include "Game.hpp"

    #define NB_CHAMPIONS 8

static const std::array<std::string, NB_CHAMPIONS> CHAMPIONS = {
    "Gules",
    "Goatier",
    "",
    "",
    "",
    "",
    "",
    "",
};

enum class ChampState {
    IDLE = 0,
    MOVEMENT = 1,
    AUTO_ATTACKS = 6,
    SPELL1 = 11,
    SPELL2 = 16,
    DEATH = 21,
};

struct Champion {
    Champion(const std::string& name) : name(name) {}
    std::string name;
    ChampState state = ChampState::IDLE;
};

void registerChampion(Game& game);
