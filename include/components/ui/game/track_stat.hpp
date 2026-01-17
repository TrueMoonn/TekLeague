/*
** EPITECH PROJECT, 2025
** GameTwo
** File description:
** track_stat.hpp
*/

#pragma once

#include "Game.hpp"

struct TrackStat {
    TrackStat(std::string name) : name(name) {}
    std::string name;
};

void registerTrackStat(Game& game);
