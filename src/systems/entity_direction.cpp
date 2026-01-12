/*
** EPITECH PROJECT, 2025
** GameTwo
** File description:
** entity_direction.hpp
*/

#include <ECS/DenseZipper.hpp>
#include <physic/components/velocity.hpp>
#include <physic/components/position.hpp>
#include <display/components/animation.hpp>
#include <sfml/components/sprite.hpp>

#include "components/target.hpp"
#include "components/stat_pool.hpp"
#include "configs/systems.hpp"

void entityDirection(Game& game) {
    game.createSystem("entity_direction", [&game](ECS::Registry&) {
        auto& targets = game.getComponent<Target>();
        auto& stats = game.getComponent<StatPool>();
        auto& posis = game.getComponent<addon::physic::Position2>();
        auto& vels = game.getComponent<addon::physic::Velocity2>();

        for (auto&& [goTo, pos, vel, stat] :
            ECS::DenseZipper(targets, posis, vels, stats)) {
            float dist = std::sqrt(std::pow(pos.x - goTo.x, 2) +
                std::pow(pos.y - goTo.y, 2));
            if (dist < 10) {
                vel.x = 0;
                vel.y = 0;
                continue;
            }
            mat::Vector2f dir(goTo - pos);
            float length = std::sqrt(dir.x * dir.x + dir.y * dir.y);
            dir /= length;
            vel.x = dir.x * stat.mov_speed;
            vel.y = dir.y * stat.mov_speed;
        }
    });
}
