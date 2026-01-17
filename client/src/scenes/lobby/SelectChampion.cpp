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
        switch (e) {
            case LOBBY_SELECT_CHAMP_LIST:
                game.updateScene(te::sStatus::RESUME, SCAST(SCENES::IN_LOBBY));
                game.updateScene(te::sStatus::DEACTIVATE, SCAST(SCENES::SELECT_CHAMP));
                net::SELECT_CHAMPION msg1;
                msg1.champion = 0;
                game.sendToServer(msg1.serialize());
                break;
            case LOBBY_SELECT_CHAMP_LIST + 1:
                game.updateScene(te::sStatus::RESUME, SCAST(SCENES::IN_LOBBY));
                game.updateScene(te::sStatus::DEACTIVATE, SCAST(SCENES::SELECT_CHAMP));
                net::SELECT_CHAMPION msg2;
                msg2.champion = 1;
                game.sendToServer(msg2.serialize());
                break;
            case LOBBY_SELECT_CHAMP_LIST + 2:
                game.updateScene(te::sStatus::RESUME, SCAST(SCENES::IN_LOBBY));
                game.updateScene(te::sStatus::DEACTIVATE, SCAST(SCENES::SELECT_CHAMP));
                net::SELECT_CHAMPION msg3;
                msg3.champion = 2;
                game.sendToServer(msg3.serialize());
                break;
            case LOBBY_SELECT_CHAMP_LIST + 3:
                game.updateScene(te::sStatus::RESUME, SCAST(SCENES::IN_LOBBY));
                game.updateScene(te::sStatus::DEACTIVATE, SCAST(SCENES::SELECT_CHAMP));
                net::SELECT_CHAMPION msg4;
                msg4.champion = 3;
                game.sendToServer(msg4.serialize());
                break;
        }
    });
}
