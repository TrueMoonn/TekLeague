/*
** EPITECH PROJECT, 2025
** GameTwo
** File description:
** Building.hpp
*/

#pragma once

    #include "Game.hpp"

struct Building {
    Building(const std::string& name) : name(name) {}
    std::string name;
};

void registerBuilding(Game& game);
