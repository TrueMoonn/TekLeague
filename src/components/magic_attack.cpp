/*
** EPITECH PROJECT, 2025
** GameTwo
** File description:
** stat_pool.cpp
*/

#include "components/magic_attack.hpp"

void registerMagicAttack(Game &game) {
    game.registerComponent("magic_attack",
        [&game](ECS::Entity e, const toml::table& params) {
        int ap = params["ap"].value_or(0);

        if (/*projectile touche ennemi*/bool tuer = true) {
            
        }
    });
}