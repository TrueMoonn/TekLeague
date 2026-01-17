/*
** EPITECH PROJECT, 2025
** GameTwo
** File description:
** champion.cpp
*/

#include "components/gules.hpp"

void registerGulesCmpts(Game& game) {
    game.registerComponent<Gball>("gball",
        [&game](ECS::Entity e, const toml::table& params) {
        std::size_t idx = params["idx"].value_or(0);
        game.createComponent<Gball>(e, idx);
    });
}
