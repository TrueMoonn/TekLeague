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
#include <entity_spec/components/team.hpp>

#include "components/champion.hpp"
#include "components/competences/target.hpp"
#include "entities.hpp"
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
        {game.nextEntity(eType::CHAMPION), "Gules", {200, 1100}},
    };

    ingame.on_activate = [&game](){
        auto& champs = game.getComponent<Champion>();
        auto& posis = game.getComponent<addon::physic::Position2>();
        auto& targets = game.getComponent<Target>();
        for (auto&& [_, pos, target] :
            ECS::DenseZipper(champs, posis, targets)) {
            target.x = pos.x;
            target.y = pos.y;
        }
        auto& players = game.getComponent<addon::intact::Player>();
        auto& teams = game.getComponent<addon::eSpec::Team>();
        for (auto&& [e, team, player] : ECS::IndexedDenseZipper(teams, players)) {
            const auto& players_lobby = game.getPlayers();
            for (auto& pla : players_lobby) {
                if (pla.id == game.getClientId()) {
                    if (pla.team == 1)
                        team.name = "blue";
                    else
                        team.name = "red";
                }
            }
        }
    };

    std::size_t idx = game.addScene(ingame);
    game.subForScene<te::Keys>(idx, "key_input", [&game](te::Keys keys) {
        if (keys[te::Key::Escape]) {
            // game.updateScene(te::sStatus::ACTIVATE, SCAST(SCENES::MAIN));
            // game.updateScene(te::sStatus::ACTIVATE, SCAST(SCENES::INGAME));
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
            const auto& winSize = wins.getComponent(
                static_cast<std::size_t>(SYSTEM_F)).win->getSize();

            for (auto&& [_, dir, pos] :
                ECS::DenseZipper(players, targets, posis)) {
                    dir.x = pos.x - (winSize.x / 2.f) + mouse.position.x;
                    dir.y = pos.y - (winSize.y / 2.f) + mouse.position.y;
            }
        }
    });
}
