/*
** EPITECH PROJECT, 2025
** GameTwo
** File description:
** SearchLobby.cpp
*/

#include <events.hpp>

#include "scenes/lobby.hpp"
#include "scenes.hpp"

void setSearchLobbyScene(Client& game) {
    te::Scene slobby;
    slobby.systems = {{
        {"poll_event"},  // INPUT
        {"button_states"},  // PRE UPDATE
        {"animate"},  // UPDATE
        {},  // POST UPDATE
        {"draw", "draw_text", "display"}  // RENDER
    }};

    slobby.entities = {
        {SEARCH_CREATE_LOBBY, "create_lobby", {1000.f, 260.f}},
    };

    std::size_t idx = game.addScene(slobby);
    game.subForScene<ECS::Entity>(idx, "clicked", [&game](ECS::Entity e) {
        switch (e) {
            case SEARCH_CREATE_LOBBY:
                // CREATE A LOBBY AND CONNECT TO IT
                game.updateScene(te::sStatus::DEACTIVATE, SCAST(SCENES::SEARCH_LOBBY));
                game.updateScene(te::sStatus::ACTIVATE, SCAST(SCENES::IN_LOBBY));
                break;
        }
    });
}
