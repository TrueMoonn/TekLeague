/*
** EPITECH PROJECT, 2025
** GameTwo
** File description:
** InLobby.cpp
*/

#include <SFML/Graphics/Text.hpp>
#include <cstddef>
#include <string>
#include <events.hpp>
#include <sfml/components/text.hpp>
#include <interaction/components/clickable.hpp>
#include <sfml/components/drawable.hpp>

#include "GameTool.hpp"
#include "components/ui/button.hpp"
#include "maths/Vector.hpp"
#include "physic/components/position.hpp"
#include "scenes/lobby.hpp"
#include "scenes.hpp"

static void updateLobbyInfos(Client &game) {
    auto& clics = game.getComponent<addon::intact::Clickable>();
    auto& texts = game.getComponent<addon::sfml::Text>();
    auto& draws = game.getComponent<addon::sfml::Drawable>();
    auto& buttons = game.getComponent<Button>();
    const std::string code = game.getCode();
    if (texts.hasComponent(LOBBY_CODE))
        texts.getComponent(LOBBY_CODE).setString(code.empty() ? "CODE: ------" : "CODE: " + code);
    if (game.isAdmin() && !draws.hasComponent(LOBBY_LAUNCH_GAME)) {
        game.createComponent<addon::sfml::Drawable>(LOBBY_LAUNCH_GAME);
        game.createComponent<Button>(LOBBY_LAUNCH_GAME);
        texts.getComponent(LOBBY_LAUNCH_GAME).setString("START");
     } else if (!game.isAdmin() && draws.hasComponent(LOBBY_LAUNCH_GAME)) {
        draws.removeComponent(LOBBY_LAUNCH_GAME);
        buttons.removeComponent(LOBBY_LAUNCH_GAME);
        texts.getComponent(LOBBY_LAUNCH_GAME).setString("");
    }
    const auto& players = game.getPlayers();
    std::size_t blue = 0;
    std::size_t red = 0;
    for (std::size_t i = 0; i < 6; ++i) {
        texts.getComponent(
            i + LOBBY_USER_BLUE_1).setString("...");
        game.removeEntity(LOBBY_SELECT_CHAMP_PP + i);
    }
    for (std::size_t i = 0; i < players.size(); ++i) {
        if (players[i].team == 1 && i < 3) {
            texts.getComponent(
                blue + LOBBY_USER_BLUE_1).setString(players[i].username);
            game.createEntity(LOBBY_SELECT_CHAMP_PP + blue,
                CHAMPION_PP[players[i].champion], {570.f, 410.f + (110.f * blue)});
            blue += 1;
        } else if (players[i].team == 2 && i < 3) {
            texts.getComponent(
                red + LOBBY_USER_RED_1).setString(players[i].username);
            game.createEntity(LOBBY_SELECT_CHAMP_PP + red + 3,
                CHAMPION_PP[players[i].champion], {1276.f, 405.f + (110.f * red)});
            red += 1;
        }
    }
}

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
        {LOBBY_SELECT_TEAM_BLUE, "select_blue", {650.f, 320.f}},
        {LOBBY_SELECT_TEAM_RED, "select_red", {1100.f, 320.f}},
        {LOBBY_LAUNCH_GAME, "launch_game", {1140.f, 750.f}},
        {LOBBY_SELECT_CHAMP, "champ_selection", {560.f, 740.f}},
        {LOBBY_CODE, "lobby_code", {960.0f, 320.0f}},
    };

    inlobby.on_activate = [&game]() {
        updateLobbyInfos(game);
    };

    inlobby.on_deactivate = [&game]() {
        game.removeEntity(LOBBY_LAUNCH_GAME);
        for (std::size_t i = 0; i < 6; ++i)
            game.removeEntity(LOBBY_SELECT_CHAMP_PP + i);
    };

    std::size_t idx = game.addScene(inlobby);
    game.subForScene(idx, "lobby:players_updated", [&game]() {
        updateLobbyInfos(game);
    });
    game.subForScene(idx, "game:game_starting", [&game]() {
        game.updateScene(te::sStatus::DEACTIVATE, SCAST(SCENES::MAIN));
        game.updateScene(te::sStatus::DEACTIVATE, SCAST(SCENES::LOBBY));
        game.updateScene(te::sStatus::DEACTIVATE, SCAST(SCENES::IN_LOBBY));
        game.updateScene(te::sStatus::ACTIVATE, SCAST(SCENES::HUD));
        game.updateScene(te::sStatus::ACTIVATE, SCAST(SCENES::INGAME));
    });
    game.subForScene<ECS::Entity>(idx, "clicked", [&game](ECS::Entity e) {
        switch (e) {
            case LOBBY_LAUNCH_GAME:
                game.sendStartGame();
                break;
            case LOBBY_SELECT_TEAM_BLUE:
                game.sendWantThisTeam(1);
                break;
            case LOBBY_SELECT_TEAM_RED:
                game.sendWantThisTeam(2);
                break;
            case LOBBY_SELECT_CHAMP:
                game.updateScene(te::sStatus::PAUSE, SCAST(SCENES::IN_LOBBY));
                game.updateScene(te::sStatus::ACTIVATE, SCAST(SCENES::SELECT_CHAMP));
                break;
        }
    });
}
