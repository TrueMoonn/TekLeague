/*
** EPITECH PROJECT, 2026
** TekLeague
** File description:
** on_death
*/

#include "components/on_death.hpp"
#include "ECS/Entity.hpp"

void registerOnDeath(Game& game) {
    game.registerComponent<onDeath>("on_death",
        [&game](ECS::Entity e, const toml::table& params) {
        const std::string& name = params["entity"].value_or("");
        game.createComponent<onDeath>(e, name);
    });
}
