/*
** EPITECH PROJECT, 2025
** GameTwo
** File description:
** champion.cpp
*/

#include "components/champion.hpp"
#include "ECS/Entity.hpp"

void registerChampion(Game& game) {
    game.registerComponent<Champion>("champion",
        [&game](ECS::Entity e, const toml::table& params) {
        const std::string& name = params["name"].value_or("unknown");
        game.createComponent<Champion>(e, name);
    });
}
