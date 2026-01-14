/*
** EPITECH PROJECT, 2025
** GameTwo
** File description:
** Main.cpp
*/

#include <events.hpp>
#include <ECS/DenseZipper.hpp>
#include <sfml/components/text.hpp>
#include <sfml/components/focus.hpp>

#include "scenes/main.hpp"
#include "ECS/DenseSA.hpp"
#include "GameTool.hpp"
#include "scenes.hpp"

void setMainScene(Client& game) {
    te::Scene main;
    main.systems = {{
        {"poll_event"},  // INPUT
        {"button_states"},  // PRE UPDATE
        {"animate"},  // UPDATE
        {},  // POST UPDATE
        {"draw", "draw_text", "display"}  // RENDER
    }};

    main.entities = {
        {MAIN_BUTTON_PLAY, "button", {760.f, 550.f}},
        {MAIN_BUTTON_SETTINGS, "button", {760.f, 700.f}},
        {MAIN_BUTTON_QUIT, "button", {760.f, 850.f}},
        {MAIN_LOGO, "logo", {30.f, 30.f}},
        {MAIN_BACKGROUND, "main_bg"},
        {MAIN_USERNAME, "text_input", {1400.f, 50.f}},
    };

    std::size_t idx = game.addScene(main);
    game.subForScene<te::Keys>(idx, "key_input", [&game](te::Keys keys) {
        if (keys[te::Key::Enter]) {
            GET_ENTITY_CMPT(
                game.getComponent<addon::sfml::Focus>(),
                MAIN_USERNAME).focus = false;
            game.client_name = GET_ENTITY_CMPT(
                game.getComponent<addon::sfml::Text>(), MAIN_USERNAME).str;
        }
    });
    game.subForScene<te::Keys>(idx, "key_input", [&game](te::Keys keys) {
        auto &text = game.getComponent<addon::sfml::Text>();
        auto &focuss = game.getComponent<addon::sfml::Focus>();

        for (auto&& [txt, focus] : ECS::DenseZipper(text, focuss)) {
            if (!focus.focus)
                continue;
            if (keys[te::Backspace] && !txt.str.empty())
                txt.str.pop_back();
            if (txt.str.size() >= 20)
                continue;
            for (int key = te::A; key <= te::Z; key += 1)
                if (keys[key])
                    txt.str.push_back(keys[te::LeftShift] ? 'A' : 'a' + key);

            for (int key = te::num0; key <= te::num9; key += 1)
                    if (keys[key])
                        txt.str.push_back('0' + key - te::num0);

            if (keys[te::Slash]) txt.str.push_back('/');
            if (keys[te::Add]) txt.str.push_back('+');
            if (keys[te::Multiply]) txt.str.push_back('*');
            if (keys[te::Period]) txt.str.push_back('.');
            if (keys[te::Comma]) txt.str.push_back(',');
            if (keys[te::Space]) txt.str.push_back(' ');
        }
    });
    game.subForScene<ECS::Entity>(idx, "clicked", [&game](ECS::Entity e) {
        switch (e) {
            case MAIN_BUTTON_PLAY:
                game.updateScene(te::sStatus::PAUSE, SCAST(SCENES::MAIN));
                game.updateScene(te::sStatus::ACTIVATE, SCAST(SCENES::LOBBY));
                game.updateScene(te::sStatus::ACTIVATE, SCAST(SCENES::SEARCH_LOBBY));
                break;
            case MAIN_BUTTON_SETTINGS:
                game.updateScene(te::sStatus::ACTIVATE, SCAST(SCENES::MAIN));
                game.updateScene(te::sStatus::ACTIVATE, SCAST(SCENES::PARAMETERS));
                break;
            case MAIN_BUTTON_QUIT:
                game.emit("closed");
                break;
            case MAIN_USERNAME:
                auto& focus = GET_ENTITY_CMPT(
                    game.getComponent<addon::sfml::Focus>(), MAIN_USERNAME);
                focus.focus = true;
                break;
        }
    });
}
