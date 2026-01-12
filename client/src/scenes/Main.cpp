/*
** EPITECH PROJECT, 2025
** GameTwo
** File description:
** Main.cpp
*/

#include <events.hpp>
#include <display/components/animation.hpp>

#include "scenes/main.hpp"
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
        {MAIN_BUTTON_PLAY, "button", {760.f, 550.f}},
        {MAIN_BUTTON_SETTINGS, "button", {760.f, 700.f}},
        {MAIN_BUTTON_QUIT, "button", {760.f, 850.f}},
        {MAIN_LOGO, "logo", {30.f, 30.f}},
        {MAIN_BACKGROUND, "main_bg"},
    };

    std::size_t idx = game.addScene(main);
    game.subForScene<ECS::Entity>(idx, "clicked", [&game](ECS::Entity e) {
        switch (e) {
            case MAIN_BUTTON_PLAY:
                // Search for lobby (connect to a server)
                // game.activateScene(SCAST(SCENES::SEARCH_SERVER));
                // game.pauseScene(SCAST(SCENES::MAIN));
                break;
            case MAIN_BUTTON_SETTINGS:
                // game.activateScene(SCAST(SCENES::SETTINGS));
                // game.pauseScene(SCAST(SCENES::MAIN));
                break;
            case MAIN_BUTTON_QUIT:
                game.emit("closed");
                break;
        }
    });
}
