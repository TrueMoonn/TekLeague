/*
** EPITECH PROJECT, 2025
** GameTwo
** File description:
** Wait.cpp
*/

#include "scenes.hpp"

void setWaitScene(Game& game) {
    te::Scene wait;
    wait.systems = {{
        {},  // INPUT
        {},  // PRE UPDATE
        {},  // UPDATE
        {},  // POST UPDATE
        {}  // RENDER
    }};

    wait.entities = {};

    std::size_t idx = game.addScene(wait);
}
