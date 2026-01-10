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
#include <physic/components/position.hpp>
#include <interaction/components/player.hpp>

#include "scenes.hpp"
#include "components/directed.hpp"
#include "components/stat_pool.hpp"


void setInGameScene(Game& game) {
    te::Scene ingame;
    ingame.systems = {{
        {"poll_event"},  // INPUT
        {},  // PRE UPDATE
        {"animate", "entity_direction", "movement2"},  // UPDATE
        {"follow_player", "champion_movement"},  // POST UPDATE
        {"draw", "display"}  // RENDER
    }};

    ingame.entities = {
        {game.nextEntity(eType::MAP), "sumoners_rift"},
        {game.nextEntity(eType::CHAMPION), "ethan", {150, 7100}}
    };

    std::size_t idx = game.addScene(ingame);
    game.subForScene<te::Keys>(idx, "key_input", [&game](te::Keys keys) {
        if (keys[te::Key::A]) {
            auto& players = game.getComponent<addon::intact::Player>();
            auto& stats = game.getComponent<StatPool>();
            // static te::Timestamp();

        }
    });
    game.subForScene<te::Mouse>(idx, "mouse_input", [&game](te::Mouse mouse) {
        if (mouse.type[te::MouseEvent::MouseRight]) {
            static te::Timestamp delta(0.02f);
            if (!delta.checkDelay())
                return;
            auto& directeds = game.getComponent<Directed>();
            auto& posis = game.getComponent<addon::physic::Position2>();
            auto& players = game.getComponent<addon::intact::Player>();

            for (auto&& [_, dir, pos] :
                ECS::DenseZipper(players, directeds, posis)) {
                    dir.x = pos.x - (1280.f / 2.f) + mouse.position.x;
                    dir.y = pos.y - (720.f / 2.f) + mouse.position.y;
            }
        }
    });
}
