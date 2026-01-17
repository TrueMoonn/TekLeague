/*
** EPITECH PROJECT, 2026
** TekLeague
** File description:
** circle_hitbox
*/
#include "components/circle_hitbox.hpp"
#include "ECS/Entity.hpp"

void registerCircleHitbox(Game& game) {
    game.registerComponent<CircleHitbox>("circle_hitbox",
        [&game](ECS::Entity e, const toml::table& params) {
        float radius = params["radius"].value_or(0);
        bool push = params["pushback"].value_or(false);
        game.createComponent<CircleHitbox>(e, radius, push);
    });
}
