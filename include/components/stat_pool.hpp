/*
** EPITECH PROJECT, 2025
** GameTwo
** File description:
** stat_pool.hpp
*/

#pragma once

    #include "Game.hpp"

struct StatPool {
    StatPool(int hp, int hp_max, int hp_regen, int ad_, int ap_, int as, int atkr, int cdr, int ar, int mr, int mv) :
        ad(ad_), ap(ap_), atk_speed(as), atk_range(atkr), cdr(cdr),
        armor(ar), magic_resist(mr), mov_speed(mv) {};
    int ad;
    int ap;
    int atk_speed;
    int atk_range;
    int cdr;
    int armor;
    int magic_resist;
    int mov_speed;
};

void registerStatPool(Game& game);
