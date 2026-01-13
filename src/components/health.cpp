/*
** EPITECH PROJECT, 2025
** GameTwo
** File description:
** team.cpp
*/

#include "components/health.hpp"

void registerHealth(Game& game) {
    game.registerComponent<Health>("health",
        [&game](ECS::Entity e, const toml::table& params) {
        int amount = params["amount"].value_or(0);
        int max = params["max"].value_or(0);
        int regen = params["regen"].value_or(0);
        game.createComponent<Health>(e, amount, max, regen);
    });
}
