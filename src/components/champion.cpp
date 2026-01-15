/*
** EPITECH PROJECT, 2025
** GameTwo
** File description:
** champion.cpp
*/

#include "components/champion.hpp"
#include "ECS/Entity.hpp"

void registerChampion(te::GameTool& game) {
    game.registerComponent<Champion>("champion",
        [&game](ECS::Entity e, const toml::table &) {
        game.createComponent<Champion>(e);
    });
}
