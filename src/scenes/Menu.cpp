/*
** EPITECH PROJECT, 2025
** GameTwo
** File description:
** Menu.cpp
*/

#include <events.hpp>

#include "scenes.hpp"

void setMenuScene(Game& game) {
    te::Scene menu;
    menu.systems = {{
        {"poll_event"},  // INPUT
        {},  // PRE UPDATE
        {"animate"},  // UPDATE
        {},  // POST UPDATE
        {"draw", "display"}  // RENDER
    }};

    menu.entities = {};

    std::size_t idx = game.addScene(menu);
    game.subForScene<te::Keys>(idx, "key_input", [&game](te::Keys keys) {
        if (keys[te::Key::P]) {
            std::cout << "GO TO INGAME\n";
            game.deactivateScene(SCAST(SCENES::MENU));
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
