/*
** EPITECH PROJECT, 2026
** GameTwo
** File description:
** directed.cpp
*/

#include "components/directed.hpp"

void registerDirected(Game& game) {
    game.registerComponent<Directed>("directed",
        [&game](ECS::Entity e, const toml::table&) {
        game.createComponent<Directed>(e);
    });
}
