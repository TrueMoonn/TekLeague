/*
** EPITECH PROJECT, 2025
** GameTwo
** File description:
** xp.cpp
*/

#include <ECS/Entity.hpp>

#include "components/stats/xp.hpp"

void registerXp(te::GameTool& game) {
    game.registerComponent<Xp>("xp",
        [&game](ECS::Entity e, const toml::table &params) {
        std::size_t amount = params["xp"].value_or(0);
        game.createComponent<Xp>(e, amount);
    });
}
