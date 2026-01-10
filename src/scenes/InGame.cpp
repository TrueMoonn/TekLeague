/*
** EPITECH PROJECT, 2026
** GameTwo
** File description:
** InGame.cpp
*/

#include <ECS/DenseZipper.hpp>

#include <events.hpp>
#include <clock.hpp>
#include <maths/Vector.hpp>
#include <physic/components/velocity.hpp>
#include <physic/components/position.hpp>
#include <interaction/components/player.hpp>
#include <sfml/components/sprite.hpp>

#include "scenes.hpp"
#include "components/stat_pool.hpp"


void setInGameScene(Game& game) {
    te::Scene ingame;
    ingame.systems = {{
        {"poll_event"},  // INPUT
        {},  // PRE UPDATE
        {"animate", "movement2"},  // UPDATE
        {"follow_player"},  // POST UPDATE
        {"draw", "display"}  // RENDER
    }};

    ingame.entities = {
        {game.nextEntity(MAP), "sumoners_rift"},
        {game.nextEntity(PLAYER), "ethan", {150, 7100}}
    };

    std::size_t idx = game.addScene(ingame);
    game.subForScene<te::Keys>(idx, "key_input", [&game](te::Keys keys) {
    });
    game.subForScene<te::Mouse>(idx, "mouse_input", [&game](te::Mouse ev) {
        if (ev.type[te::MouseEvent::MouseRight]) {
            static te::Timestamp delta(0.02f);
            if (!delta.checkDelay())
                return;
            auto& stats = game.getComponent<StatPool>();
            auto& vels = game.getComponent<addon::physic::Velocity2>();
            auto& posis = game.getComponent<addon::physic::Position2>();
            auto& players = game.getComponent<addon::intact::Player>();

            for (auto&& [_, vel, pos, stat] :
                ECS::DenseZipper(players, vels, posis, stats)) {
                    mat::Vector2f actual_click;
                    //  MAGIC VALUE, TO CHANGE WHEN WINDOW SIZE CHANGE
                    actual_click.x = pos.x - (1280.f / 2.f) + ev.position.x;
                    actual_click.y = pos.x - (720.f / 2.f) + ev.position.y;
                    mat::Vector2f dir(actual_click - pos);
                    float length = std::sqrt(dir.x * dir.x + dir.y * dir.y);
                    dir /= length;
                    vel.x = dir.x * stat.mov_speed;
                    vel.y = dir.y * stat.mov_speed;
            }
        }
    });
}
