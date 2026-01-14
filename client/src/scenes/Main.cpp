/*
** EPITECH PROJECT, 2025
** GameTwo
** File description:
** Main.cpp
*/

#include <events.hpp>
#include <sfml/components/text.hpp>
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
    };

    // toml::table table;
    // table.insert_or_assign("font", "client/assets/images/ui/main_font.otf");
    // table.insert_or_assign("string", "PLAY");
    // toml::array offset;
    // offset.push_back(150);
    // offset.push_back(5);
    // table.insert_or_assign("offset", offset);
    // table.insert_or_assign("size", 60);
    // game.createComponent("text", MAIN_BUTTON_PLAY, table);

    // toml::table table1;
    // table1.insert_or_assign("font", "client/assets/images/ui/main_font.otf");
    // table1.insert_or_assign("string", "SETTINGS");
    // toml::array offset1;
    // offset1.push_back(100);
    // offset1.push_back(5);
    // table1.insert_or_assign("offset", offset1);
    // table1.insert_or_assign("size", 60);
    // game.createComponent("text", MAIN_BUTTON_SETTINGS, table1);

    // toml::table table2;
    // table2.insert_or_assign("font", "client/assets/images/ui/main_font.otf");
    // table2.insert_or_assign("string", "QUIT");
    // toml::array offset2;
    // offset2.push_back(150);
    // offset2.push_back(5);
    // table2.insert_or_assign("offset", offset2);
    // table2.insert_or_assign("size", 60);
    // game.createComponent("text", MAIN_BUTTON_QUIT, table2);

    std::size_t idx = game.addScene(main);
    game.subForScene<ECS::Entity>(idx, "clicked", [&game](ECS::Entity e) {
        switch (e) {
            case MAIN_BUTTON_PLAY:
                {
                    net::GET_ALL_PUBLIC_LOBBIES msg;
                    game.sendToServer(msg.serialize());
                    std::println("[Client] Requesting public lobbies list...");
                }
                game.activateScene(SCAST(SCENES::SEARCH_LOBBY));
                game.pauseScene(SCAST(SCENES::MAIN));
                break;
            case MAIN_BUTTON_SETTINGS:
                game.activateScene(SCAST(SCENES::PARAMETERS));
                game.pauseScene(SCAST(SCENES::MAIN));
                break;
            case MAIN_BUTTON_QUIT:
                game.emit("closed");
                break;
        }
    });
}
