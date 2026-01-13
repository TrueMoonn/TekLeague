/*
** EPITECH PROJECT, 2025
** GameTwo
** File description:
** team.cpp
*/

#include "components/interaction/team.hpp"
#include "ECS/Entity.hpp"

void registerTeam(Game &game) {
    game.registerComponent<Team>("team", [&game](ECS::Entity e,
        const toml::table &params) {
            const std::string name = params["name"].value_or("");
            game.createComponent<Team>(e, name);
    });
}
