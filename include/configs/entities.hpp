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
};

static const std::vector<std::string> CLIENT_CONFIG_PATHS {
    /* == System == */
    "client/assets/configs/windows.toml",
    "client/assets/configs/map.toml",

    /* === CHAMPIONS === */
    "client/assets/configs/champions/ethan.toml",

    /* === UI === */
    "client/assets/configs/ui/buttons.toml",
    "client/assets/configs/ui/logo.toml",
    "client/assets/configs/ui/backgrounds.toml",
    "client/assets/configs/ui/text_input.toml",
    /* == Lobby == */
    "client/assets/configs/ui/lobby/book.toml",
    "client/assets/configs/ui/lobby/buttons.toml",
    "client/assets/configs/ui/lobby/users.toml",
    /* == Parameters == */
    "client/assets/configs/ui/parameters/background.toml",
    "client/assets/configs/ui/parameters/button.toml",
    /* == Tower Interaction == */
    "client/assets/configs/buildings/Bluetower.toml",
    "client/assets/configs/buildings/Redtower.toml",
    "client/assets/configs/buildings/zone_enemy.toml",
};

static const std::vector<std::string> MAP_PATHS {
};
