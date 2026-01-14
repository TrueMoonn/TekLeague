/*
** EPITECH PROJECT, 2025
** GameTwo
** File description:
** mana.cpp
*/

#include "components/stats/mana.hpp"
#include "ECS/Entity.hpp"
#include <toml++/impl/table.hpp>

void registerMana(Game &game) {
    game.registerComponent<Mana>("mana",
        [&game](ECS::Entity e, const toml::table &params) {
        int mana = params["mana"].value_or(0);
        int mana_max = params["mana_max"].value_or(0);
        int mana_regen = params["mana_regen"].value_or(0);
        game.createComponent<Mana>(e, mana, mana_max, mana_regen);
    });
}
