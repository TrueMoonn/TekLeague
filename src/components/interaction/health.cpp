/*
** EPITECH PROJECT, 2025
** GameTwo
** File description:
** health.cpp
*/

#include "components/stats/health.hpp"
#include "ECS/Entity.hpp"
#include <toml++/impl/table.hpp>

void Health::reduceSafely(const std::int64_t &value) {
    amount -= value > this->amount ? this->amount : value;
}

void registerHealth(Game& game) {
    game.registerComponent<Health>("health", [&game](ECS::Entity e,
        const toml::table &params) {
        int health = params["health"].value_or(0);
        int health_max = params["health_max"].value_or(0);
        int health_regen = params["health_regen"].value_or(0);
        game.createComponent<Health>(e, health, health_max, health_regen);
    });
}
