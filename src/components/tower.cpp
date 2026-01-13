/*
** EPITECH PROJECT, 2025
** GameTwo
** File description:
** champion.cpp
*/

#include "components/tower.hpp"

void registerTower(Game &game) {
    game.registerComponent("tower",
        [&game](ECS::Entity e, const toml::table& params) {
        int range = 0; //range
        int damage = 0; //damage
        int firerate = 0; //firerate
        

        if (/*projectile touche ennemi*/bool tuer = true) {
            
        }
    });
}