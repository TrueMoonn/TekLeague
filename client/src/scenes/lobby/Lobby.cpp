/*
** EPITECH PROJECT, 2025
** GameTwo
** File description:
** Lobby.cpp
*/

#include <events.hpp>
#include <print>

#include "scenes/lobby.hpp"
#include "scenes.hpp"

void setLobbyScene(Client& game) {
    te::Scene lobby;
    lobby.systems = {{
        {"poll_event"},  // INPUT
        {"button_states"},  // PRE UPDATE
        {"animate"},  // UPDATE
        {},  // POST UPDATE
        {"draw", "draw_text", "display"}  // RENDER
    }};

    lobby.entities = {
        {SEARCH_BOOKCOVER, "bookcover", {512.f, 220.f}},
        {SEARCH_BOOKLEFT, "bookleft", {542.f, 250.f}},
        {SEARCH_BOOKRIGHT, "bookright", {958.f, 250.f}},
        {SEARCH_RETURN, "sl_return_button", {562.f, 270.f}},
    };

    std::size_t idx = game.addScene(lobby);
    game.subForScene<ECS::Entity>(idx, "clicked", [&game](ECS::Entity e) {
        switch (e) {
            case SEARCH_RETURN:
                game.updateScene(te::sStatus::DEACTIVATE, SCAST(SCENES::IN_LOBBY));
                game.updateScene(te::sStatus::DEACTIVATE, SCAST(SCENES::SEARCH_LOBBY));
                game.updateScene(te::sStatus::DEACTIVATE, SCAST(SCENES::LOBBY));
                game.updateScene(te::sStatus::RESUME, SCAST(SCENES::MAIN));
                break;
        }
    });

    game.subForScene<te::Keys>(idx, "key_input", [&game](te::Keys keys) {
        if (keys[te::Key::Escape]) {
            game.updateScene(te::sStatus::DEACTIVATE, SCAST(SCENES::IN_LOBBY));
            game.updateScene(te::sStatus::DEACTIVATE, SCAST(SCENES::SEARCH_LOBBY));
            game.updateScene(te::sStatus::DEACTIVATE, SCAST(SCENES::LOBBY));
            game.updateScene(te::sStatus::RESUME, SCAST(SCENES::MAIN));
        }
    });
}
