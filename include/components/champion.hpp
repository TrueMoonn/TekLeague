/*
** EPITECH PROJECT, 2025
** GameTwo
** File description:
** champion.hpp
*/

#pragma once

    #include "Game.hpp"

enum class ChampionState {
    IDLE_BOT = 0,
    IDLE_BOT_SIDE,
    IDLE_SIDE,
    IDLE_TOP_SIDE,
    IDLE_TOP,
    MOV_BOT,
    MOV_BOT_SIDE,
    MOV_SIDE,
    MOV_TOP_SIDE,
    MOV_TOP,
};

enum DamageType : std::size_t {
    PHYSIC = 0,
    MAGICAL,
    LIMIT,
};

struct Spell {
    std::string name;
    DamageType dmgType;
    std::size_t cooldown;
    std::size_t castTime;
    std::array<std::size_t, DamageType::LIMIT> ratios;
};

struct Champion {
    // std::string name;
    // ChampionState state;
    // Spell Aspell;
    // Spell Zspell;
    short champion = 0;
};

void registerChampion(Game& game);
