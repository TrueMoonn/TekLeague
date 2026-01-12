/*
** EPITECH PROJECT, 2025
** GameTwo
** File description:
** zone_ranged.cpp
*/

#include "components/ranged_zone.hpp"
#include "ECS/Entity.hpp"
#include "maths/Vector.hpp"

void registerRangedZone(Game& game) {
    game.registerComponent<RangedZone>("proc",
        [&game](ECS::Entity e, const toml::table &params) {
            int radius = params["radius"].value_or(0);
            game.createComponent<RangedZone>(e, radius);
    });
}
