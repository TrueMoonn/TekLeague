/*
** EPITECH PROJECT, 2025
** GameTwo
** File description:
** minions.cpp
*/

#include "components/minions/minions.hpp"
#include "ECS/Entity.hpp"
#include <toml++/impl/table.hpp>

void registerMinions(Game& game) {
    game.registerComponent<Minions>("minions", [&game](ECS::Entity e, const toml::table &params) {
        std::string name = params["name"].value_or("");
        game.createComponent<Minions>(e, name);
    });
}
