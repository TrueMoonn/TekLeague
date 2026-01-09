/*
** EPITECH PROJECT, 2025
** GameTwo
** File description:
** player_movement.cpp
*/

#include <events.hpp>
#include <physic/components/velocity.hpp>
#include <physic/components/position.hpp>
#include <interaction/components/player.hpp>
#include <sfml/components/sprite.hpp>

#include "Game.hpp"
#include "components/stat_pool.cpp"

void playerMovement(Game& game) {
    game.sub<te::Mouse>("mouse_input", [&game](te::Mouse ev) {
        if (ev.type[te::MouseEvent::MouseRight]) {
            auto& stats = game.getComponent<StatPool>();
            auto& vels = game.getComponent<addon::physic::Velocity2>();
            auto& posis = game.getComponent<addon::physic::Position2>();
            auto& players = game.getComponent<addon::intact::Player>();
            auto& sprites = game.getComponent<addon::sfml::Sprite>();

            for (auto&& [_, vel, pos, stat, sp] :
                ECS::DenseZipper(players, vels, posis, stats, sprites)) {
                auto scale = sp.sp.getScale();
                if (vel.x < 0 && scale.x > 0) {
                    scale.x *= -1;
                    sp.sp.setScale(scale);
                } else if (scale <= 0) {
                    scale.x *= -1;
                    sp.sp.setScale(scale);
                }
            }
        }
    });
}
