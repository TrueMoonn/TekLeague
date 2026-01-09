/*
** EPITECH PROJECT, 2025
** GameTwo
** File description:
** champion.cpp
*/

#include "components/champion.hpp"

void registerChampion(Game& game) {
    game.registerComponent("champion",
        [&game](ECS::Entity e, const toml::table&) {
        game.createComponent<Champion>(e);
    });
}
