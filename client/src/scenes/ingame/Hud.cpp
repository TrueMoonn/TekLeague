/*
** EPITECH PROJECT, 2026
** TekLeague
** File description:
** Hud.cpp
*/

#include <ECS/DenseZipper.hpp>
#include "components/champion.hpp"
#include "interaction/components/player.hpp"
#include "scenes/lobby.hpp"
#include "sfml/components/text.hpp"

#include "scenes.hpp"
#include "scenes/ingame.hpp"

void setHudScene(Client& game) {
    std::cout << "createScene\n";
    te::Scene hud;
    hud.systems = {{
        {"poll_event"},  // INPUT
        {},  // PRE UPDATE
        {"animate"},  // UPDATE
        {"follow_player", "update_hud"},  // POST UPDATE
        {"draw", "draw_text", "display"}  // RENDER
    }};

    hud.entities = {
        {MAIN_HUD, "main_hud"},
        {CHAMP_NAME, "hud_champion_name"},
        {CHAMP_PP, "hud_champion_pp"},
        {HP_BAR, "hud_bar"},
        {HP_BAR_VAL, "hud_bar_hp"},
        {MANA_BAR, "hud_bar"},
        {MANA_BAR_VAL, "hud_bar_mana"},
    };

    hud.on_activate = [&game]() {
        auto& texts = game.getComponent<addon::sfml::Text>();
        auto& players = game.getComponent<addon::intact::Player>();
        auto& champs = game.getComponent<Champion>();
        for (auto&& [_, champ] : ECS::DenseZipper(players, champs)) {
            texts.getComponent(CHAMP_NAME).setString(champ.name);
            game.createEntity(CHAMP_ACTUAL_PP, CHAMP_NAMES_PP.at(champ.name));
        }
    };

    std::size_t idx = game.addScene(hud);
}
