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
#include <sfml/components/window.hpp>

#include "ECS/DenseSA.hpp"
#include "entities.hpp"
#include "scenes.hpp"
#include "components/target.hpp"
#include "components/ranged_zone.hpp"
#include "components/tower.hpp"


void setInGameScene(Game& game) {
    te::Scene ingame;
    ingame.systems = {{
        {"poll_event"},  // INPUT
        {"champion_movement", "target_player", "track_target"},  // PRE UPDATE
        {"animate", "entity_direction", "movement2"},  // UPDATE
        {"follow_player"},  // POST UPDATE
        {"draw", "display"}  // RENDER
    }};

    ingame.entities = {
        {game.nextEntity(eType::MAP), "sumoners_rift"},
        {game.nextEntity(eType::BUILDINGS), "tower", {200, 6300}},
        {game.nextEntity(eType::CHAMPION), "ethan", {150, 7100}},
        {game.nextEntity(eType::BUILDINGS), "zone_left_enemy", {200, 6350}}
    };

    std::size_t idx = game.addScene(ingame);
    game.subForScene<te::Keys>(idx, "key_input", [&game](te::Keys keys) {
        if (keys[te::Key::Escape]) {
            game.deactivateScene(SCAST(SCENES::INGAME));
            game.activateScene(SCAST(SCENES::CLIENT));
        }
        // if (keys[te::Key::A]) {
        //     auto& players = game.getComponent<addon::intact::Player>();
        //     auto& stats = game.getComponent<StatPool>();
        //     static te::Timestamp();
        // }
    });
    game.subForScene<te::Mouse>(idx, "mouse_input", [&game](te::Mouse mouse) {
        if (mouse.type[te::MouseEvent::MouseRight]) {
            static te::Timestamp delta(0.02f);
            if (!delta.checkDelay())
                return;
            auto& targets = game.getComponent<Target>();
            auto& posis = game.getComponent<addon::physic::Position2>();
            auto& players = game.getComponent<addon::intact::Player>();
            auto& wins = game.getComponent<addon::sfml::Window>();
            const auto& winSize = GET_ENTITY_CMPT(wins,
                static_cast<std::size_t>(SYSTEM_F)).win->getSize();

            for (auto&& [_, dir, pos] :
                ECS::DenseZipper(players, targets, posis)) {
                    dir.x = pos.x - (winSize.x / 2.f) + mouse.position.x;
                    dir.y = pos.y - (winSize.y / 2.f) + mouse.position.y;
            }
        }
    });
}
