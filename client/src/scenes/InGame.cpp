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
#include "components/directed.hpp"


void setInGameScene(Client& game) {
    te::Scene ingame;
    ingame.systems = {{
        {"poll_event"},  // INPUT
        {},  // PRE UPDATE
        {"animate"},  // UPDATE
        {"follow_player", "champion_movement"},  // POST UPDATE
        {"draw", "display"}  // RENDER
    }};

    ingame.entities = {};

    std::size_t idx = game.addScene(ingame);
    game.subForScene<te::Keys>(idx, "key_input", [&game](te::Keys keys) {
        if (keys[te::Key::Escape]) {
            game.deactivateScene(SCAST(SCENES::INGAME));
            game.activateScene(SCAST(SCENES::MAIN));
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
            auto& wins = game.getComponent<addon::sfml::Window>();
            const auto& winSize = GET_ENTITY_CMPT(wins,
                static_cast<std::size_t>(SYSTEM_F)).win->getSize();

            for (auto&& [_, dir, pos] :
                ECS::DenseZipper(players, directeds, posis)) {
                    dir.x = pos.x - (winSize.x / 2.f) + mouse.position.x;
                    dir.y = pos.y - (winSize.y / 2.f) + mouse.position.y;
            }
        }
    });
}
