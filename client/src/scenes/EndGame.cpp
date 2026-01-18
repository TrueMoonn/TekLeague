/*
** EPITECH PROJECT, 2026
** TekLeague
** File description:
** EndGame.cpp
*/

#include <print>
#include <optional>
#include <string>
#include <vector>

#include <events.hpp>
#include <sfml/components/text.hpp>
#include <sfml/components/window.hpp>
#include <physic/components/position.hpp>

#include "my.hpp"
#include "scenes/end_game.hpp"
#include "scenes.hpp"
#include "Client.hpp"
#include "sfml/components/drawable.hpp"

static void returnToMenu(Client& game) {
    game.sendLeaveLobby();
    game.clearPlayers();
    game.setCode("");
    game.updateScene(te::sStatus::DEACTIVATE, SCAST(SCENES::END_GAME));
    auto& win = game.getComponent<addon::sfml::Window>().getComponent(SYSTEM_F).win;
    win->setView(win->getDefaultView());
    game.updateScene(te::sStatus::ACTIVATE, SCAST(SCENES::MAIN));
    game.setPendingWinningTeam(std::nullopt);
    game.setGameState(LobbyGameState::PRE_GAME);
}

void setEndGameScene(Client& game) {
    te::Scene end_scene;
    end_scene.systems = {{
        {"poll_event"},        // INPUT
        {"button_states"},     // PRE UPDATE
        {"animate"},           // UPDATE
        {"center_end_popup"},  // POST UPDATE
        {"draw", "draw_text", "display"}  // RENDER
    }};

    end_scene.entities = {
        {END_POPUP_BG, "end_popup_bg", {960.f, 540.f}},
        {END_POPUP_TITLE, "end_popup_title", {960.f, 480.f}},
        {END_POPUP_BUTTON, "end_popup_button", {960.f, 640.f}},
    };

    game.createSystem("center_end_popup", [&game](ECS::Registry&) {
        auto& positions = game.getComponent<addon::physic::Position2>();
        const auto view = game.getComponent<addon::sfml::Window>()
            .getComponent(SYSTEM_F).win->getView();
        const auto center = view.getCenter();

        if (positions.hasComponent(END_POPUP_BG)) {
            positions.getComponent(END_POPUP_BG).x = center.x;
            positions.getComponent(END_POPUP_BG).y = center.y;
        }
        if (positions.hasComponent(END_POPUP_TITLE)) {
            positions.getComponent(END_POPUP_TITLE).x = center.x;
            positions.getComponent(END_POPUP_TITLE).y = center.y - 80.f;
        }
        if (positions.hasComponent(END_POPUP_BUTTON)) {
            positions.getComponent(END_POPUP_BUTTON).x = center.x;
            positions.getComponent(END_POPUP_BUTTON).y = center.y + 120.f;
        }
    });

    end_scene.on_activate = [&game]() {
        auto& texts = game.getComponent<addon::sfml::Text>();
        auto& positions = game.getComponent<addon::physic::Position2>();
        auto& draws = game.getComponent<addon::sfml::Drawable>();
        auto& win = game.getComponent<addon::sfml::Window>()
            .getComponent(SYSTEM_F).win;
        win->setView(win->getDefaultView());
        const auto center = win->getView().getCenter();

        for (ECS::Entity e = 0; e < ONDEATH_END; ++e) {
            if (e == END_POPUP_BG || e == END_POPUP_TITLE || e == END_POPUP_BUTTON)
                continue;

            if (draws.hasComponent(e))
                draws.getComponent(e).drawable = 0;

            if (texts.hasComponent(e)) {
                auto &txt = texts.getComponent(e);
                txt.setString("");
            }
        }

        if (positions.hasComponent(END_POPUP_BG)) {
            positions.getComponent(END_POPUP_BG).x = center.x;
            positions.getComponent(END_POPUP_BG).y = center.y;
        }
        if (positions.hasComponent(END_POPUP_TITLE)) {
            positions.getComponent(END_POPUP_TITLE).x = center.x;
            positions.getComponent(END_POPUP_TITLE).y = center.y - 80.f;
        }
        if (positions.hasComponent(END_POPUP_BUTTON)) {
            positions.getComponent(END_POPUP_BUTTON).x = center.x;
            positions.getComponent(END_POPUP_BUTTON).y = center.y + 120.f;
        }

        if (texts.hasComponent(END_POPUP_TITLE)) {
            texts.getComponent(END_POPUP_TITLE).setString(
                game.getEndGameMessage());
        }
        if (texts.hasComponent(END_POPUP_BUTTON)) {
            texts.getComponent(END_POPUP_BUTTON).setString("MAIN MENU");
        }
    };

    std::size_t idx = game.addScene(end_scene);

    game.sub("game:game_end", [&game]() {
        game.deactivateAllScenes();
        game.updateScene(te::sStatus::ACTIVATE, SCAST(SCENES::END_GAME));
    });

    game.subForScene<ECS::Entity>(idx, "clicked", [&game](ECS::Entity e) {
        if (e == END_POPUP_BUTTON) {
            returnToMenu(game);
        }
    });

    game.subForScene<te::Keys>(idx, "key_input", [&game](te::Keys keys) {
        if (keys[te::Key::Escape]) {
            returnToMenu(game);
        }
    });
}
