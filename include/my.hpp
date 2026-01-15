/*
** EPITECH PROJECT, 2025
** GameTwo
** File description:
** my.hpp
*/

#pragma once

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
    "",
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
