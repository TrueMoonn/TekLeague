/*
** EPITECH PROJECT, 2025
** GameTwo
** File description:
** SearchLobby.cpp
*/

#include <print>

#include <events.hpp>
#include <sfml/components/text.hpp>
#include <sfml/components/drawable.hpp>

#include "GameTool.hpp"
#include "Network/generated_messages.hpp"
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
    };

    slobby.on_activate = [&game]() {
        auto& texts = game.getComponent<addon::sfml::Text>();
        texts.getComponent(SEARCH_CREATE_LOBBY).setString("CREATE");
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
    game.subForScene<std::string, bool>(idx, "lobby:created",
        [&game](const std::string& lobby_code, bool) {
        game.updateScene(te::sStatus::ACTIVATE, SCAST(SCENES::IN_LOBBY));
        game.updateScene(te::sStatus::DEACTIVATE, SCAST(SCENES::SEARCH_LOBBY));
    });
    game.subForScene(idx, "lobby:bad_code", [&game]() {
        std::println("[SearchLobby] Error: Invalid lobby code!");
    });
    game.subForScene(idx, "lobby:full", [&game]() {
        std::println("[SearchLobby] Error: Lobby is full!");
    });
    game.subForScene<ECS::Entity>(idx, "clicked", [&game](ECS::Entity e) {
        switch (e) {
            case SEARCH_CREATE_LOBBY:
                net::CREATE_LOBBY msg;
                game.sendToServer(msg.serialize());
                break;
        }
    });
}
