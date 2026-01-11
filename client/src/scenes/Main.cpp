/*
** EPITECH PROJECT, 2025
** GameTwo
** File description:
** Main.cpp
*/

#include <events.hpp>

#include "scenes.hpp"

void setMainScene(Game& game) {
    te::Scene main;
    main.systems = {{
        {"poll_event"},  // INPUT
        {},  // PRE UPDATE
        {"animate"},  // UPDATE
        {},  // POST UPDATE
        {"draw", "display"}  // RENDER
    }};

    main.entities = {};

    std::size_t idx = game.addScene(main);
    game.subForScene<te::Keys>(idx, "key_input", [&game](te::Keys keys) {
        if (keys[te::Key::Escape])
            game.emit("closed");
        if (keys[te::Key::P]) {
            game.deactivateScene(SCAST(SCENES::MAIN));
            game.activateScene(SCAST(SCENES::INGAME));
        }
    });
    game.subForScene<te::Mouse>(idx, "mouse_input", [&game](te::Mouse mouse) {
        for (auto& ev : mouse.type) {
            if (ev) {
                std::cout << "click IN MENU\n";
                std::cout << mouse.position.x << " " << mouse.position.y << std::endl;
            }
        }
    });
}
