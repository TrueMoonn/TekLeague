/*
** EPITECH PROJECT, 2026
** TekLeague
** File description:
** on_death
*/

#pragma once

    #include "Game.hpp"

struct onDeath {
    onDeath(const std::string &entityName) : name(entityName) {}
    std::string name;
};

void registerOnDeath(Game& game);
