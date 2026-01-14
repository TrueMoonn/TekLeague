/*
** EPITECH PROJECT, 2025
** GameTwo
** File description:
** kill_all_entity.hpp
*/
#include <ECS/DenseZipper.hpp>
#include <physic/components/velocity.hpp>
#include <physic/components/position.hpp>
#include <display/components/animation.hpp>
#include <sfml/components/sprite.hpp>

#include "ECS/Registry.hpp"
#include "components/target.hpp"
#include "components/stat_pool.hpp"
#include "configs/systems.hpp"

void killAllEntity(Game &game) {
    game.createSystem("kill_all_entity",
        [&game](ECS::Registry &) {
        for (auto& e : game.getAllEntityToKill()) {
            game.removeEntity(e);
        }
    });
}
