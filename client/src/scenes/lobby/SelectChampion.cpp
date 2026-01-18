/*
** EPITECH PROJECT, 2025
** GameTwo
** File description:
** SelectChampion.cpp
*/

#include <events.hpp>
#include <sfml/components/text.hpp>

#include "Network/generated_messages.hpp"
#include "scenes/lobby.hpp"
#include "scenes.hpp"

void setChampionSelection(Client& game) {
    te::Scene cselect;
    cselect.systems = {{
        {"poll_event"},  // INPUT
        {"button_states"},  // PRE UPDATE
        {"animate"},  // UPDATE
        {},  // POST UPDATE
        {"draw", "draw_text", "display"}  // RENDER
    }};

    cselect.entities = {
        {LOBBY_SELECT_CHAMP_BG, "champ_select_bg", {760.f, 340.f}},
        {LOBBY_SELECT_CHAMP_LIST + 0, "select_champ", {810.f, 390.f}},
        {LOBBY_SELECT_CHAMP_LIST + 1, "select_champ", {810.f, 540.f}},
        {LOBBY_SELECT_CHAMP_LIST + 2, "select_champ", {960.f, 390.f}},
        {LOBBY_SELECT_CHAMP_LIST + 3, "select_champ", {960.f, 540.f}},
    };

    cselect.on_activate = [&game]() {
        auto& texts = game.getComponent<addon::sfml::Text>();
        texts.getComponent(LOBBY_SELECT_CHAMP_LIST + 0).setString("Gules");
        texts.getComponent(LOBBY_SELECT_CHAMP_LIST + 1).setString("Goatier");
        texts.getComponent(LOBBY_SELECT_CHAMP_LIST + 2).setString("Ttheodore");
        texts.getComponent(LOBBY_SELECT_CHAMP_LIST + 3).setString("Croly");
    };

    std::size_t idx = game.addScene(cselect);
    game.subForScene<ECS::Entity>(idx, "clicked", [&game](ECS::Entity e) {
        if (e >= LOBBY_SELECT_CHAMP_LIST && e <= LOBBY_SELECT_CHAMP_LIST + 3) {
            game.updateScene(te::sStatus::RESUME, SCAST(SCENES::IN_LOBBY));
            game.updateScene(te::sStatus::DEACTIVATE, SCAST(SCENES::SELECT_CHAMP));
            net::SELECT_CHAMPION msg;
            msg.champion = e - LOBBY_SELECT_CHAMP_LIST;
            game.sendToServer(msg.serialize());
        }
    });
}
