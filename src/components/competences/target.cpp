/*
** EPITECH PROJECT, 2026
** GameTwo
** File description:
** target.cpp
*/

#include "components/competences/target.hpp"

void registerTarget(Game& game) {
    game.registerComponent<Target>("target",
        [&game](ECS::Entity e, const toml::table&) {
        game.createComponent<Target>(e);
    });
}
