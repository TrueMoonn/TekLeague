/*
** EPITECH PROJECT, 2025
** GameTwo
** File description:
** auto_track.hpp
*/

#pragma once

    #include "ECS/Entity.hpp"
#include "Game.hpp"
#include "maths/Vector.hpp"

struct AutoTrack {
    AutoTrack(float vel, bool track) : vel(vel), track(track) {}
    float vel;
    ECS::Entity toFollow = 0;
    bool track;
};

void registerAutoTrack(Game& game);
