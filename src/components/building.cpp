/*
** EPITECH PROJECT, 2025
** GameTwo
** File description:
** building.cpp
*/

#include "components/building.hpp"
#include "ECS/Entity.hpp"

void registerBuilding(Game& game) {
    game.registerComponent<Building>("building",
        [&game](ECS::Entity e, const toml::table& params) {
        const std::string& name = params["name"].value_or("unknown");
        game.createComponent<Building>(e, name);
    });
}
