/*
** EPITECH PROJECT, 2025
** GameTwo
** File description:
** InLobby.cpp
*/

#include <events.hpp>

#include "scenes.hpp"

void setInLobbyScene(Client& game) {
    te::Scene inlobby;
    inlobby.systems = {{
        {"poll_event"},  // INPUT
        {"button_states"},  // PRE UPDATE
        {"animate"},  // UPDATE
        {},  // POST UPDATE
        {"draw", "draw_text", "display"}  // RENDER
    }};

    inlobby.entities = {

    };

    // récupérer les infos avec game.getLobbyData();
    std::size_t idx = game.addScene(inlobby);
}
