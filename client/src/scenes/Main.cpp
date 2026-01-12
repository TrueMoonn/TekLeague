/*
** EPITECH PROJECT, 2025
** GameTwo
** File description:
** Main.cpp
*/

#include <events.hpp>
#include <display/components/animation.hpp>

#include "entities.hpp"
#include "scenes.hpp"

void setMainScene(Client& game) {
    te::Scene main;
    main.systems = {{
        {"poll_event"},  // INPUT
        {"button_states"},  // PRE UPDATE
        {"animate"},  // UPDATE
        {},  // POST UPDATE
        {"draw", "display"}  // RENDER
    }};

    main.entities = {
        {SCAST(MENU_BEGIN + 0), "button", {760.f, 550.f}},
        {SCAST(MENU_BEGIN + 1), "button", {760.f, 700.f}},
        {SCAST(MENU_BEGIN + 2), "button", {760.f, 850.f}},
    };

    std::size_t idx = game.addScene(main);
    game.subForScene<ECS::Entity>(idx, "clicked", [&game](ECS::Entity e) {
        if (e == SCAST(MENU_BEGIN + 0)) {
            // Search for lobby (connect to a server)
            // game.activateScene(SCAST(SCENES::SEARCH_SERVER));
            // game.pauseScene(SCAST(SCENES::MAIN));
        }
    });
    game.subForScene<te::Keys>(idx, "key_input", [&game](te::Keys keys) {
        if (keys[te::Key::Escape])
            game.emit("closed");
    });
}
