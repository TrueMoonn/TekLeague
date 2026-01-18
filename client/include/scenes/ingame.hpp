/*
** EPITECH PROJECT, 2026
** TekLeague
** File description:
** ingame.hpp
*/

#pragma once

    #include <cstddef>
    #include <string>
    #include <unordered_map>

    #include "entities.hpp"

enum igHud : std::size_t {
    MAIN_HUD = HUD_BEGIN,
    CHAMP_NAME,
    CHAMP_PP,
    CHAMP_ACTUAL_PP,
    HP_BAR,
    HP_BAR_VAL,
    MANA_BAR,
    MANA_BAR_VAL,
};

static const std::unordered_map<std::string, std::string> CHAMP_NAMES_PP = {
    {"Gules", "gules_pp"},
    {"Goatier", "goatier_pp"},
    {"Croly", "croly_pp"},
    {"Ttheodore", "ttheodore_pp"},
};
