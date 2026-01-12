/*
** EPITECH PROJECT, 2025
** GameTwo
** File description:
** Main.cpp
*/

#include <events.hpp>

#include "entities.hpp"
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

    main.entities = {
        {MENU_BEGIN + 0, "play_button", {800, 800}}
    };

    std::size_t idx = game.addScene(main);
    game.subForScene<ECS::Entity>(idx, "clicked", [&game](ECS::Entity e) {
        if (e == MENU_BEGIN + 0) {
            // Search for lobby (connect to a server)
            // game.activateScene(SCAST(SCENES::SEARCH_SERVER));
            // game.pauseScene(SCAST(SCENES::MAIN));
            std::cout << "clicked\n";
        }
    });
    game.subForScene<te::Keys>(idx, "key_input", [&game](te::Keys keys) {
        if (keys[te::Key::Escape])
            game.emit("closed");
    });
    game.subForScene<te::Mouse>(idx, "mouse_input", [&game](te::Mouse keys) {
        if (keys.type[te::MouseEvent::MouseLeft]) {
            std::cout << keys.position.x << " " << keys.position.y << std::endl;
        }
    });
}
