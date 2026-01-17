/*
** EPITECH PROJECT, 2025
** GameTwo
** File description:
** my.hpp
*/

#pragma once

    #include "maths/Vector.hpp"
    #include <array>
    #include <string>

    #define BLUE_POS_X 750
    #define BLUE_POS_Y 1800

    #define RED_POS_X 8192 - 750
    #define RED_POS_Y 1800

    #define NB_CHAMPIONS 8

static const std::array<std::string, NB_CHAMPIONS> CHAMPIONS = {
    "Gules",
    "Goatier",
    "TTheodore",
    "",
    "",
    "",
    "",
    "",
};

    #define NB_TEAMS 3

static const std::array<std::string, NB_TEAMS> TEAMS = {
    "monster",
    "blue",
    "red",
};

enum class ActionIG {
    MOVEMENT = 0,
    AA,
    SPELL1,
    SPELL2,
    BACK,
    SHOP,
};

 #define NB_BUILDINGS 6

static const std::array<std::pair<std::string, mat::Vector2f>, NB_BUILDINGS> BUILDINGS = {{
    {"tower_blue", mat::Vector2f(1400.0f, 1200.0f)},
    {"tower_blue", mat::Vector2f(1100.0f, 1800.0f)},
    {"tower_blue", mat::Vector2f(1400.0f, 2400.0f)},

    {"tower_red", mat::Vector2f(6792.0f, 1200.0f)},
    {"tower_red", mat::Vector2f(7092.0f, 1800.0f)},
    {"tower_red", mat::Vector2f(6792.0f, 2400.0f)}
}};

  #define NB_CREATURES 4
static const std::array<std::pair<std::string, mat::Vector2f>, NB_CREATURES> CREATURES = {{
    {"range_blue_creap", mat::Vector2f(1400.0f, 1200.0f)},
    {"range_blue_creap", mat::Vector2f(1400.0f, 2400.0f)},

    {"range_red_creap", mat::Vector2f(6792.0f, 1200.0f)},
    {"range_red_creap", mat::Vector2f(6792.0f, 2400.0f)},
}};
