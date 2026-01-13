/*
** EPITECH PROJECT, 2025
** GameTwo
** File description:
** team.hpp
*/

#pragma once
#include "Game.hpp"

struct Team {
    Team(const std::string &teamName) : name(teamName){};

    std::string name;
};

void registerTeam(Game& game);
