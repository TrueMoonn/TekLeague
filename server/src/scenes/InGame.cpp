/*
** EPITECH PROJECT, 2026
** GameTwo
** File description:
** InGame.cpp
*/

#include "entities.hpp"
#include "scenes.hpp"


void setInGameScene(Game& game) {
    te::Scene ingame;
    ingame.systems = {{
        {},  // INPUT
        {},  // PRE UPDATE
        {"entity_direction", "movement2"},  // UPDATE
        {},  // POST UPDATE
        {}  // RENDER
    }};

    ingame.entities = {
        {game.nextEntity(eType::MAP), "sumoners_rift"},
        {game.nextEntity(eType::CHAMPION), "ethan", {150, 7100}}
    };

    std::size_t idx = game.addScene(ingame);
}
