/*
** EPITECH PROJECT, 2026
** GameTwo
** File description:
** InGame.cpp
*/

#include <cstdint>

#include <ECS/DenseZipper.hpp>
#include <events.hpp>
#include <clock.hpp>
#include <maths/Vector.hpp>
#include <physic/components/position.hpp>
#include <interaction/components/player.hpp>
#include <sfml/components/window.hpp>
#include <entity_spec/components/team.hpp>

#include "GameTool.hpp"
#include "Network/generated_messages.hpp"
#include "components/competences/target.hpp"
#include "entities.hpp"
#include "my.hpp"
#include "scenes.hpp"

void setInGameScene(Client& game) {
    te::Scene ingame;
    ingame.systems = {{
        {"poll_event"},  // INPUT
        {"champion_movement", "update_sticky", "minion_animation", "track_stat"},  // PRE UPDATE
        {"animate", "play_sound"},  // UPDATE
        {"follow_player"},  // POST UPDATE
        {"draw", "display"}  // RENDER
    }};

    ingame.entities = {
        {game.nextEntity(eType::MAP), "sumoners_rift_ground"},
        {game.nextEntity(eType::MAP), "sumoners_rift_nash_zone"},
        {game.nextEntity(eType::MAP), "sumoners_rift_jungle"},
        {game.nextEntity(eType::MAP), "sumoners_rift_walls"},
    };

    std::size_t idx = game.addScene(ingame);
    game.subForScene<te::Keys>(idx, "key_input", [&game](te::Keys keys) {
        if (keys[te::Key::A]) {
            net::CLIENT_INPUTS msg;
            msg.mouse_x = game.mpos.x;
            msg.mouse_y = game.mpos.y;
            msg.actions = static_cast<uint8_t>(ActionIG::SPELL1);
            game.sendToServer(msg.serialize());
        }
        if (keys[te::Key::Z]) {
            net::CLIENT_INPUTS msg;
            msg.mouse_x = game.mpos.x;
            msg.mouse_y = game.mpos.y;
            msg.actions = static_cast<uint8_t>(ActionIG::SPELL2);
            game.sendToServer(msg.serialize());
        }
    });
    game.subForScene<te::Mouse>(idx, "mouse_input", [&game](te::Mouse mouse) {
        const auto& winSize = game.getComponent<addon::sfml::Window>()
            .getComponent(static_cast<std::size_t>(SYSTEM_F)).win->getSize();
        auto& posis = game.getComponent<addon::physic::Position2>();
        auto& players = game.getComponent<addon::intact::Player>();
        for (ECS::Entity e = 0; e < ONDEATH_END; ++e) {
            if (!players.hasComponent(e) || !posis.hasComponent(e))
                continue;
            auto& pos = posis.getComponent(e);
            game.mpos.x = pos.x - (winSize.x / 2.f) + mouse.position.x;
            game.mpos.y = pos.y - (winSize.y / 2.f) + mouse.position.y;
        }

        if (mouse.type[te::MouseEvent::MouseRight]) {
            if (!game.inputLimit.checkDelay())
                return;
            net::CLIENT_INPUTS msg;
            msg.mouse_x = game.mpos.x;
            msg.mouse_y = game.mpos.y;
            msg.target = 0;
            msg.actions = static_cast<uint8_t>(ActionIG::MOVEMENT);
            game.sendToServer(msg.serialize());
        }
    });
    game.subForScene<ECS::Entity>(idx, "clicked", [&game](ECS::Entity e) {
        if (!game.inputLimit.checkDelay())
            return;
        auto& players = game.getComponent<addon::intact::Player>();
        auto& teams = game.getComponent<addon::eSpec::Team>();
        auto& targets = game.getComponent<Target>();
        if (teams.hasComponent(e)) {
            for (ECS::Entity pe = 0; pe < ONDEATH_END; ++pe) {
                if (!players.hasComponent(pe) || !teams.hasComponent(pe) || !targets.hasComponent(pe))
                    continue;
                auto& team = teams.getComponent(pe);
                if (team.name != teams.getComponent(e).name) {
                    net::CLIENT_INPUTS msg;
                    msg.mouse_x = game.mpos.x;
                    msg.mouse_y = game.mpos.y;
                    msg.target = e;
                    msg.actions = static_cast<uint8_t>(ActionIG::AA);
                    game.sendToServer(msg.serialize());
                }
            }
        }
    });
}
