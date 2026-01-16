/*
** EPITECH PROJECT, 2026
** TekLeague
** File description:
** spawn_death_entity
*/

#include <ECS/DenseZipper.hpp>
#include <ECS/Registry.hpp>

#include <physic/components/position.hpp>

#include "components/on_death.hpp"
#include "configs/systems.hpp"

void spawnDeathEntity(Game &game) {
    game.createSystem("spawn_death_entity", [&game](ECS::Registry&) {
        auto& pos = game.getComponent<addon::physic::Position2>();
        auto& on_death = game.getComponent<onDeath>();

        for (auto e : game.getAllEntityToKill()) {
            if (!on_death.hasComponent(e))
                continue;
            auto &od = on_death.getComponent(e);
            if (pos.hasComponent(e)) {
                auto &posval = pos.getComponent(e);
                std::cout << "making at " << posval.x << "," << posval.y << std::endl;
                game.createEntity(game.nextEntity(eType::ONDEATH), od.name,
                    {posval.x, posval.y});
            } else {
                game.createEntity(game.nextEntity(eType::ONDEATH), od.name);
            }
        }
    });
}
