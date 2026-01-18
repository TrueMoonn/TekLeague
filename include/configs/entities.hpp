/*
** EPITECH PROJECT, 2025
** GameTwo
** File description:
** entities.hpp
*/

#pragma once

    #include <string>
    #include <vector>

static const std::vector<std::string> CONFIG_PATHS {
    "assets/configs/champions/ethan.toml",
    "assets/configs/champions/goatier.toml",
    "assets/configs/champions/gules.toml",
    "assets/configs/champions/croly.toml",
    "assets/configs/champions/ttheodore.toml",
    //
    "assets/configs/buildings/tower.toml",
    "assets/configs/buildings/nexus.toml",
    "assets/configs/creatures/nash.toml"
};

static const std::vector<std::string> CLIENT_CONFIG_PATHS {
    /* == System == */
    "client/assets/configs/windows.toml",
    "client/assets/configs/map.toml",

    /* === CHAMPIONS === */
    "client/assets/configs/champions/ethan.toml",
    "client/assets/configs/champions/goatier.toml",
    "client/assets/configs/champions/gules.toml",
    "client/assets/configs/champions/ttheodore.toml",
    "client/assets/configs/champions/croly.toml",

    /* === UI === */
    "client/assets/configs/ui/buttons.toml",
    "client/assets/configs/ui/logo.toml",
    "client/assets/configs/ui/backgrounds.toml",
    "client/assets/configs/ui/text_input.toml",
    /* == InGame == */
    "client/assets/configs/hud/champion_info.toml",
    /* == Lobby == */
    "client/assets/configs/ui/lobby/book.toml",
    "client/assets/configs/ui/lobby/buttons.toml",
    "client/assets/configs/ui/lobby/users.toml",
    /* == Parameters == */
    "client/assets/configs/ui/parameters/background.toml",
    "client/assets/configs/ui/parameters/button.toml",
    /* == End Game == */
    "client/assets/configs/ui/end_game.toml",
    /* == Buildings == */
    "client/assets/configs/buildings/tower.toml",
    "client/assets/configs/buildings/nexus.toml",
    "client/assets/configs/creatures/nash.toml"
};

static const std::vector<std::string> MAP_PATHS {
};
