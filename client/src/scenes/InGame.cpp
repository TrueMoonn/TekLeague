/*
** EPITECH PROJECT, 2026
** GameTwo
** File description:
** InGame.cpp
*/

#include <ECS/DenseZipper.hpp>

#include <cstdint>
#include <events.hpp>
#include <clock.hpp>
#include <maths/Vector.hpp>
#include <physic/components/position.hpp>
#include <interaction/components/player.hpp>
#include <sfml/components/window.hpp>
#include <entity_spec/components/team.hpp>

#include "Network/generated_messages.hpp"
#include "components/champion.hpp"
#include "components/competences/target.hpp"
#include "entities.hpp"
#include "my.hpp"
#include "scenes.hpp"


void setInGameScene(Client& game) {
    te::Scene ingame;
    ingame.systems = {{
        {"poll_event"},  // INPUT
        {"champion_movement", "auto_attacks", "track_target", "deal_damage", "kill_all_entity"},  // PRE UPDATE
        {"animate", "entity_direction", "movement2"},  // UPDATE
        {"follow_player"},  // POST UPDATE
        {"draw", "display"}  // RENDER
    }};

    ingame.entities = {
        {game.nextEntity(eType::MAP), "sumoners_rift_ground"},
        {game.nextEntity(eType::MAP), "sumoners_rift_nash_zone"},
        {game.nextEntity(eType::MAP), "sumoners_rift_jungle"},
        {game.nextEntity(eType::MAP), "sumoners_rift_walls"},
        {game.nextEntity(eType::BUILDINGS), "tower_blue", {1400, 1000}},
        {game.nextEntity(eType::BUILDINGS), "tower_red", {6792, 1000}},
        {game.nextEntity(eType::BUILDINGS), "zone_left_enemy", {6792, 1000}},
    };

    ingame.on_activate = [&game](){};

    std::size_t idx = game.addScene(ingame);
    game.subForScene<te::Keys>(idx, "key_input", [&game](te::Keys keys) {
        if (keys[te::Key::Escape]) {
            game.updateScene(te::sStatus::ACTIVATE, SCAST(SCENES::PARAMETERS));
        }

        if (keys[te::Key::A]) {
        }
    });
    game.subForScene<te::Mouse>(idx, "mouse_input", [&game](te::Mouse mouse) {
        if (mouse.type[te::MouseEvent::MouseRight]) {
            static te::Timestamp delta(0.02f);
            if (!delta.checkDelay())
                return;
            auto& posis = game.getComponent<addon::physic::Position2>();
            auto& players = game.getComponent<addon::intact::Player>();
            const auto& winSize = game.getComponent<addon::sfml::Window>()
                .getComponent(static_cast<std::size_t>(SYSTEM_F)).win->getSize();

            for (auto&& [_, pos] : ECS::DenseZipper(players, posis)) {
                net::CLIENT_INPUTS msg;
                msg.mouse_x = pos.x - (winSize.x / 2.f) + mouse.position.x;
                msg.mouse_y = pos.y - (winSize.y / 2.f) + mouse.position.y;
                msg.actions = static_cast<uint8_t>(ActionIG::MOVEMENT);
                game.sendToServer(msg.serialize());
            }
        }
    });
}
