/*
** EPITECH PROJECT, 2025
** GameTwo
** File description:
** SearchLobby.cpp
*/

#include <cstring>
#include <print>

#include <events.hpp>
#include <sfml/components/text.hpp>
#include <sfml/components/drawable.hpp>

#include "GameTool.hpp"
#include "components/ui/button.hpp"
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
        {SEARCH_CREATE_LOBBY, "create_lobby", {560.f, 700.f}},
        {SEARCH_LOBBY_LIST + 0, "lobby_list", {1000.f, 400.f}},
        {SEARCH_LOBBY_LIST + 1, "lobby_list", {1000.f, 510.f}},
        {SEARCH_LOBBY_LIST + 2, "lobby_list", {1000.f, 620.f}},
        {SEARCH_LOBBY_LIST + 3, "lobby_list", {1000.f, 730.f}},
    };

    slobby.on_activate = [&game]() {
        auto& texts = game.getComponent<addon::sfml::Text>();
        auto& draws = game.getComponent<addon::sfml::Drawable>();
        auto& buttons = game.getComponent<Button>();
        texts.getComponent(SEARCH_CREATE_LOBBY).setString("CREATE");
        for (std::size_t i = 0; i < 4; ++i) {
            if (draws.hasComponent(SEARCH_LOBBY_LIST + i)) {
                draws.removeComponent(SEARCH_LOBBY_LIST + i);
                buttons.removeComponent(SEARCH_LOBBY_LIST + i);
            }
        }
    };

    std::size_t idx = game.addScene(slobby);
    game.subForScene(idx, "lobby:lobbies_list", [&game]() {
        const auto& lobbies = game.getCachedLobbiesList();
        auto& draws = game.getComponent<addon::sfml::Drawable>();
        auto& buttons = game.getComponent<Button>();
        auto& texts = game.getComponent<addon::sfml::Text>();
        for (std::size_t i = 0; i < 4; ++i) {
            if (draws.hasComponent(SEARCH_LOBBY_LIST + i)) {
                draws.removeComponent(SEARCH_LOBBY_LIST + i);
                buttons.removeComponent(SEARCH_LOBBY_LIST + i);
                texts.getComponent(SEARCH_LOBBY_LIST + i).setString("");
            }
        }
        for (std::size_t i = 0; i < 4 && i < lobbies.size(); ++i) {
            texts.getComponent(SEARCH_LOBBY_LIST + i).setString(lobbies[i]);
            game.createComponent<addon::sfml::Drawable>(SEARCH_LOBBY_LIST + i);
            game.createComponent<Button>(SEARCH_LOBBY_LIST + i);
        }
    });
    game.subForScene<std::string, bool>(idx, "lobby:created",
        [&game](const std::string& lobby_code, bool) {
        game.updateScene(te::sStatus::ACTIVATE, SCAST(SCENES::IN_LOBBY));
        game.updateScene(te::sStatus::DEACTIVATE, SCAST(SCENES::SEARCH_LOBBY));
    });
    game.subForScene<std::string, bool>(idx, "lobby:joined",
        [&game](const std::string& lobby_code, bool) {
        game.updateScene(te::sStatus::ACTIVATE, SCAST(SCENES::IN_LOBBY));
        game.updateScene(te::sStatus::DEACTIVATE, SCAST(SCENES::SEARCH_LOBBY));
    });
    game.subForScene(idx, "lobby:bad_code", [&game]() {
        // std::println("[SearchLobby] Error: Invalid lobby code!");
    });
    game.subForScene(idx, "lobby:full", [&game]() {
        // std::println("[SearchLobby] Error: Lobby is full!");
    });
    game.subForScene<ECS::Entity>(idx, "clicked", [&game](ECS::Entity e) {
        if (e >= SEARCH_LOBBY_LIST && e < SEARCH_LOBBY_LIST + 4) {
            std::string lobby_code = game.getCachedLobbiesList()[e - SEARCH_LOBBY_LIST];
            game.sendJoinLobby(lobby_code);
            return;
        }
        switch (e) {
            case SEARCH_CREATE_LOBBY:
                game.sendCreateLobby();
                break;
        }
    });
}
