/*
** EPITECH PROJECT, 2025
** GameTwo
** File description:
** team.cpp
*/

#include "components/stats/gold.hpp"

void registerGold(Game& game) {
    game.registerComponent<Gold>("gold",
        [&game](ECS::Entity e, const toml::table& params) {
        int amount = params["amount"].value_or(0);
        game.createComponent<Gold>(e, amount);
    });
}
