/*
** EPITECH PROJECT, 2025
** GameTwo
** File description:
** auto_attack.cpp
*/

#include <string>

#include "components/competences/auto_attack.hpp"

void registerAttack(Game& game) {
    game.registerComponent<Attack>("attack",
        [&game](ECS::Entity e, const toml::table& params) {
            const std::string& attack = params["name"].value_or("unkown");
            bool aa = params["auto"].value_or(false);
            game.createComponent<Attack>(e, attack, aa);
    });
}
