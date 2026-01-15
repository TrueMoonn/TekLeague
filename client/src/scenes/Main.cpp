/*
** EPITECH PROJECT, 2025
** GameTwo
** File description:
** Main.cpp
*/

#include <SFML/Graphics/Drawable.hpp>
#include <events.hpp>
#include <ECS/DenseZipper.hpp>
#include <sfml/components/text.hpp>
#include <sfml/components/focus.hpp>
#include <sfml/components/drawable.hpp>
#include <Network/generated_messages.hpp>
#include <print>

#include "scenes/main.hpp"
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

    main.on_activate = [&game]() {
        auto& texts = game.getComponent<addon::sfml::Text>();
        texts.getComponent(MAIN_BUTTON_PLAY).setString("PLAY");
        texts.getComponent(MAIN_BUTTON_SETTINGS).setString("SETTINGS");
        texts.getComponent(MAIN_BUTTON_QUIT).setString("QUIT");
    };

    main.on_pause = [&game] {
        auto& texts = game.getComponent<addon::sfml::Text>();
        auto& draws = game.getComponent<addon::sfml::Drawable>();
        texts.getComponent(MAIN_BUTTON_PLAY).setString("");
        texts.getComponent(MAIN_BUTTON_SETTINGS).setString("");
        texts.getComponent(MAIN_BUTTON_QUIT).setString("");
        draws.removeComponent(MAIN_BUTTON_PLAY);
        draws.removeComponent(MAIN_BUTTON_SETTINGS);
        draws.removeComponent(MAIN_BUTTON_QUIT);
    };

    main.on_resume = [&game]() {
        auto& texts = game.getComponent<addon::sfml::Text>();
        texts.getComponent(MAIN_BUTTON_PLAY).setString("PLAY");
        texts.getComponent(MAIN_BUTTON_SETTINGS).setString("SETTINGS");
        texts.getComponent(MAIN_BUTTON_QUIT).setString("QUIT");
        game.createComponent<addon::sfml::Drawable>(MAIN_BUTTON_PLAY);
        game.createComponent<addon::sfml::Drawable>(MAIN_BUTTON_SETTINGS);
        game.createComponent<addon::sfml::Drawable>(MAIN_BUTTON_QUIT);
    };

    std::size_t idx = game.addScene(main);
    game.subForScene(idx, "lobby:logged_in", [&game]() {
        // ok
    });
    game.subForScene<te::Keys>(idx, "key_input", [&game](te::Keys keys) {
        if (keys[te::Key::Enter]) {
            game.getComponent<addon::sfml::Focus>().
                getComponent(MAIN_USERNAME).focus = false;
            game.setUsername(game.getComponent<addon::sfml::Text>()
                .getComponent(MAIN_USERNAME).getString());
            game.sendLogin(game.getUsername());
        }
    });
    game.subForScene<te::Keys>(idx, "key_input", [&game](te::Keys keys) {
        auto &text = game.getComponent<addon::sfml::Text>();
        auto &focuss = game.getComponent<addon::sfml::Focus>();

        for (auto&& [txt, focus] : ECS::DenseZipper(text, focuss)) {
            if (!focus.focus)
                continue;
            std::string str = txt.getString();
            if (keys[te::Backspace] && !str.empty())
                str.pop_back();
            if (str.size() >= 20) {
                txt.setString(str);
                continue;
            }
            for (int key = te::A; key <= te::Z; key += 1)
                if (keys[key])
                    str.push_back(keys[te::LeftShift] ? 'A' : 'a' + key);

            for (int key = te::num0; key <= te::num9; key += 1)
                    if (keys[key])
                        str.push_back('0' + key - te::num0);

            if (keys[te::Slash]) str.push_back('/');
            if (keys[te::Add]) str.push_back('+');
            if (keys[te::Multiply]) str.push_back('*');
            if (keys[te::Period]) str.push_back('.');
            if (keys[te::Comma]) str.push_back(',');
            if (keys[te::Space]) str.push_back(' ');
            txt.setString(str);
        }
    });
    game.subForScene<ECS::Entity>(idx, "clicked", [&game](ECS::Entity e) {
        switch (e) {
            case MAIN_BUTTON_PLAY:
                game.updateScene(te::sStatus::PAUSE, SCAST(SCENES::MAIN));
                game.updateScene(te::sStatus::ACTIVATE, SCAST(SCENES::LOBBY));
                game.updateScene(te::sStatus::ACTIVATE, SCAST(SCENES::SEARCH_LOBBY));
                game.sendGetPublicLobbies();
                break;
            case MAIN_BUTTON_SETTINGS:
                game.updateScene(te::sStatus::PAUSE, SCAST(SCENES::MAIN));
                game.updateScene(te::sStatus::ACTIVATE, SCAST(SCENES::PARAMETERS));
                break;
            case MAIN_BUTTON_QUIT:
                game.emit("closed");
                break;
            case MAIN_USERNAME:
                auto& focus = game.getComponent<addon::sfml::Focus>()
                    .getComponent(MAIN_USERNAME);
                focus.focus = true;
                break;
        }
    });
}
