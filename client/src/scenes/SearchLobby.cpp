/*
** EPITECH PROJECT, 2025
** GameTwo
** File description:
** Main.cpp
*/

#include <events.hpp>

#include "scenes/search_lobby.hpp"
#include "scenes.hpp"

void setSearchLobbyScene(Client& game) {
    te::Scene slobby;
    slobby.systems = {{
        {"poll_event"},  // INPUT
        {"button_states"},  // PRE UPDATE
        {"animate"},  // UPDATE
        {},  // POST UPDATE
        {"draw", "display"}  // RENDER
    }};

    slobby.entities = {
        {SEARCH_BOOKCOVER, "bookcover", {512.f, 220.f}},
        {SEARCH_BOOKLEFT, "bookleft", {542.f, 250.f}},
        {SEARCH_BOOKRIGHT, "bookright", {958.f, 250.f}},
    };

    std::size_t idx = game.addScene(slobby);
    game.subForScene<ECS::Entity>(idx, "clicked", [&game](ECS::Entity e) {
    });
    game.subForScene<te::Keys>(idx, "key_input", [&game](te::Keys keys) {
        if (keys[te::Key::Escape]) {
            game.resumeScene(SCAST(SCENES::MAIN));
            game.deactivateScene(SCAST(SCENES::SEARCH_LOBBY));
        }
    });
}
