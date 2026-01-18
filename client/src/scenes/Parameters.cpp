/*
** EPITECH PROJECT, 2025
** GameTwo
** File description:
** Parameters.cpp
*/

#include <events.hpp>
#include <sfml/components/text.hpp>
#include <sfml/components/drawable.hpp>

#include "scenes/parameters.hpp"
#include "scenes.hpp"

void setParametersScenes(Client& game) {
    te::Scene slobby;
    slobby.systems = {{
        {"poll_event"},  // INPUT
        {"button_states"},  // PRE UPDATE
        {"animate"},  // UPDATE
        {},  // POST UPDATE
        {"draw", "draw_text", "display"}  // RENDER
    }};

    slobby.entities = {
        {PARAM_BG, "param_bg", {660.f, 340.f}},
        {PARAM_BUTTON_BACK, "param_button", {810.f, 380.f}},
        {PARAM_BUTTON_KEYS, "param_button", {810.f, 490.f}},
        {PARAM_BUTTON_QUIT, "param_button", {810.f, 600.f}},
    };

    slobby.on_activate = [&game]() {
        auto& texts = game.getComponent<addon::sfml::Text>();
        texts.getComponent(PARAM_BUTTON_BACK).setString("RESUME");
        texts.getComponent(PARAM_BUTTON_KEYS).setString("KEYBOARD");
        texts.getComponent(PARAM_BUTTON_QUIT).setString("QUIT");
    };

    std::size_t idx = game.addScene(slobby);
    game.subForScene<ECS::Entity>(idx, "clicked", [&game](ECS::Entity e) {
        switch (e) {
            case PARAM_BUTTON_BACK:
                game.updateScene(te::sStatus::RESUME, SCAST(SCENES::MAIN));
                game.updateScene(te::sStatus::DEACTIVATE,
                    SCAST(SCENES::PARAMETERS));
                break;
            case PARAM_BUTTON_KEYS:
                // KEY BINDINGS
                break;
            case PARAM_BUTTON_QUIT:
                game.emit("closed");
                break;
        }
    });
    game.subForScene<te::Keys>(idx, "key_input", [&game](te::Keys keys) {
        if (keys[te::Key::Escape]) {
            game.updateScene(te::sStatus::RESUME, SCAST(SCENES::MAIN));
            game.updateScene(te::sStatus::DEACTIVATE,
                SCAST(SCENES::PARAMETERS));
        }
    });
}
