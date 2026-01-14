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
    "client/assets/configs/windows.toml",
    "client/assets/configs/map.toml",

    /* === CHAMPIONS === */
    "client/assets/configs/champions/ethan.toml",

    /* === UI === */
    "client/assets/configs/ui/buttons.toml",
    "client/assets/configs/ui/logo.toml",
    "client/assets/configs/ui/backgrounds.toml",
    /* == Search == */
    "client/assets/configs/ui/search_lobby/book.toml",
    "client/assets/configs/ui/search_lobby/button.toml",
    /* == Parameters == */
    "client/assets/configs/ui/parameters/background.toml",
    "client/assets/configs/ui/parameters/button.toml",

    "assets/config/buildings/tower.toml",
    "assets/config/buildings/zone_enemy.toml",
};

static const std::vector<std::string> MAP_PATHS {
};
