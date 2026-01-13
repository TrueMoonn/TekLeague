/*
** EPITECH PROJECT, 2025
** GameTwo
** File description:
** auto_attack.cpp
*/

#include "components/auto_attack.hpp"

void registerAutoAttack(Game& game) {
    game.registerComponent<AutoAttack>("auto_attack",
        [&game](ECS::Entity e, const toml::table &) {
            game.createComponent<AutoAttack>(e);
    });
}
