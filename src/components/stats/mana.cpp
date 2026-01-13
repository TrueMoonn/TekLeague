/*
** EPITECH PROJECT, 2025
** GameTwo
** File description:
** team.cpp
*/

#include "components/stats/mana.hpp"

void registerMana(Game& game) {
    game.registerComponent<Mana>("mana",
        [&game](ECS::Entity e, const toml::table& params) {
        int amount = params["amount"].value_or(0);
        int max = params["max"].value_or(0);
        int regen = params["regen"].value_or(0);
        game.createComponent<Mana>(e, amount, max, regen);
    });
}
