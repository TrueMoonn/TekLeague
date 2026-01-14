/*
** EPITECH PROJECT, 2025
** GameTwo
** File description:
** InLobby.cpp
*/

#include <SFML/Graphics/Text.hpp>
#include <events.hpp>
#include <sfml/components/text.hpp>

#include "ECS/DenseZipper.hpp"
#include "scenes/lobby.hpp"
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
        {LOBBY_USER_BLUE_1, "user_blue_side_lobby", {560.f, 400.f}},
        {LOBBY_USER_BLUE_2, "user_blue_side_lobby", {560.f, 510.f}},
        {LOBBY_USER_BLUE_3, "user_blue_side_lobby", {560.f, 620.f}},
        {LOBBY_USER_RED_1, "user_red_side_lobby", {980.f, 400.f}},
        {LOBBY_USER_RED_2, "user_red_side_lobby", {980.f, 510.f}},
        {LOBBY_USER_RED_3, "user_red_side_lobby", {980.f, 620.f}},
    };

    inlobby.on_activate = [&game]() {
        const auto& lobby_names = game.getLobbyData().getPlayersInLobby();
        for (std::size_t i = 0; i < lobby_names.size(); ++i) {
            game.getComponent<addon::sfml::Text>().getComponent(
                i + LOBBY_USER_BLUE_1).setString(lobby_names[i].username);
        }
    };

    std::size_t idx = game.addScene(inlobby);
}
