/*
** EPITECH PROJECT, 2025
** TekLeague
** File description:
** lobby.hpp
*/

#pragma once

    #include <array>
#include <vector>
    #include <string>

    #include "my.hpp"
    #include "entities.hpp"

    #define TEMP_SCENE 500 +

    #define SEARCH_BOOKCOVER static_cast<std::size_t>(TEMP_SCENE\
        MENU_BEGIN + 0)
    #define SEARCH_BOOKLEFT static_cast<std::size_t>(TEMP_SCENE\
        MENU_BEGIN + 1)
    #define SEARCH_BOOKRIGHT static_cast<std::size_t>(TEMP_SCENE\
        MENU_BEGIN + 2)
    #define SEARCH_RETURN static_cast<std::size_t>(TEMP_SCENE\
        MENU_BEGIN + 3)
    #define SEARCH_CREATE_LOBBY static_cast<std::size_t>(TEMP_SCENE\
        MENU_BEGIN + 4)
    #define SEARCH_LOBBY_LIST static_cast<std::size_t>(TEMP_SCENE\
        MENU_BEGIN + 5)

    #define LOBBY_USER_BLUE_1 static_cast<std::size_t>(TEMP_SCENE\
        MENU_BEGIN + 9)
    #define LOBBY_USER_BLUE_2 static_cast<std::size_t>(TEMP_SCENE\
        MENU_BEGIN + 10)
    #define LOBBY_USER_BLUE_3 static_cast<std::size_t>(TEMP_SCENE\
        MENU_BEGIN + 11)
    #define LOBBY_USER_RED_1 static_cast<std::size_t>(TEMP_SCENE\
        MENU_BEGIN + 12)
    #define LOBBY_USER_RED_2 static_cast<std::size_t>(TEMP_SCENE\
        MENU_BEGIN + 13)
    #define LOBBY_USER_RED_3 static_cast<std::size_t>(TEMP_SCENE\
        MENU_BEGIN + 14)
    #define LOBBY_LAUNCH_GAME static_cast<std::size_t>(TEMP_SCENE\
        MENU_BEGIN + 15)
    #define LOBBY_SELECT_TEAM_BLUE static_cast<std::size_t>(TEMP_SCENE\
        MENU_BEGIN + 16)
    #define LOBBY_SELECT_TEAM_RED static_cast<std::size_t>(TEMP_SCENE\
        MENU_BEGIN + 17)
    #define LOBBY_SELECT_CHAMP static_cast<std::size_t>(TEMP_SCENE\
        MENU_BEGIN + 18)

    #define LOBBY_SELECT_CHAMP_BG static_cast<std::size_t>(TEMP_SCENE\
        MENU_BEGIN + 19)
    #define LOBBY_SELECT_CHAMP_PP static_cast<std::size_t>(TEMP_SCENE\
        MENU_BEGIN + 20)
    #define LOBBY_SELECT_CHAMP_LIST static_cast<std::size_t>(TEMP_SCENE\
        MENU_BEGIN + 26)

static const std::array<std::string, NB_CHAMPIONS> CHAMPION_PP = {
    "gules_pp",
    "goatier_pp",
    "ttheodore_pp",
    "croly_pp",
};
