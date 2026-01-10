/*
** EPITECH PROJECT, 2025
** GameTwo
** File description:
** champion.hpp
*/

#pragma once

    #include "Game.hpp"

enum ChampionFrames {
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

struct Champion {
    short champion = 0;
};

void registerChampion(Game& game);
