/*
** EPITECH PROJECT, 2026
** TekLeague
** File description:
** spawn_death_entity
*/

#include <ECS/DenseZipper.hpp>
#include <ECS/Registry.hpp>

#include <physic/components/position.hpp>

#include "ECS/Entity.hpp"
#include "components/on_death.hpp"
#include "configs/systems.hpp"
#include "entities.hpp"

void spawnDeathEntity(Game &game) {
    game.createSystem("spawn_death_entity", [&game](ECS::Registry&) {
        auto& pos = game.getComponent<addon::physic::Position2>();
        auto& on_death = game.getComponent<onDeath>();

        for (auto e : game.getAllEntityToKill()) {
            if (!on_death.hasComponent(e)) {
                continue;
            }
            auto &od = on_death.getComponent(e);
            if (pos.hasComponent(e)) {
                auto &posval = pos.getComponent(e);
                ECS::Entity next_e = game.nextEntity(eType::ONDEATH);
                game.createEntity(next_e, od.name,
                    {posval.x, posval.y});
                game.entities_queue.emplace_back(next_e, od.name);
            } else {
                ECS::Entity next_e = game.nextEntity(eType::ONDEATH);
                game.createEntity(next_e, od.name);
                game.entities_queue.emplace_back(next_e, od.name);
            }
        }
    });
}
