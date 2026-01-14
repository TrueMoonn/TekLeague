/*
** EPITECH PROJECT, 2025
** GameTwo
** File description:
** SearchLobby.cpp
*/

#include <events.hpp>
#include <print>

#include "scenes/search_lobby.hpp"
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
        {SEARCH_BOOKCOVER, "bookcover", {512.f, 220.f}},
        {SEARCH_BOOKLEFT, "bookleft", {542.f, 250.f}},
        {SEARCH_BOOKRIGHT, "bookright", {958.f, 250.f}},
        {SEARCH_RETURN, "sl_return_button", {562.f, 270.f}},
    };

    std::size_t idx = game.addScene(slobby);

    game.subForScene<std::vector<std::string>>(idx, "lobby:lobbies_list",
        [&game](const std::vector<std::string>& lobbies) {
            std::println("[SearchLobby] Received {} public lobbies:", lobbies.size());
            for (const auto& code : lobbies) {
                std::println("  - {}", code);
            }
            // TODO(Jules): Display lobbies as clickable buttons (if clicked, try to join (envoie au server SendJoinLobby(code du lobby)))
        });

    game.subForScene<std::string>(idx, "lobby:joined",
        [&game](const std::string& lobby_code) {
            std::println("[SearchLobby] Successfully joined lobby: {}", lobby_code);
            // TODO(Jules): Navigate to lobby scene
        });

    game.subForScene(idx, "lobby:bad_code", [&game]() {
        std::println("[SearchLobby] Error: Invalid lobby code!");
        // TODO(Jules): Show error message
    });

    game.subForScene(idx, "lobby:full", [&game]() {
        std::println("[SearchLobby] Error: Lobby is full!");
        // TODO(Jules): Show error message
    });

    game.subForScene<ECS::Entity>(idx, "clicked", [&game](ECS::Entity e) {
        switch (e) {
            case SEARCH_RETURN:
                game.resumeScene(SCAST(SCENES::MAIN));
                game.deactivateScene(SCAST(SCENES::SEARCH_LOBBY));
                break;
        }
    });

    game.subForScene<te::Keys>(idx, "key_input", [&game](te::Keys keys) {
        if (keys[te::Key::Escape]) {
            game.resumeScene(SCAST(SCENES::MAIN));
            game.deactivateScene(SCAST(SCENES::SEARCH_LOBBY));
        }
    });
}
