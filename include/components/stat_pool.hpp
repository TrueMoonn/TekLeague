/*
** EPITECH PROJECT, 2025
** GameTwo
** File description:
** stat_pool.hpp
*/

#pragma once

    #include "Game.hpp"

struct StatPool {
    StatPool(int ad_, int ap_, int as, int ar, int mr, int mv) :
        ad(ad_), ap(ap_), atk_speed(as),
        armor(ar), magic_resist(mr), mov_speed(mv) {};
    int ad;
    int ap;
    int atk_speed;
    int armor;
    int magic_resist;
    int mov_speed;
};

void registerStatPool(Game& game);
