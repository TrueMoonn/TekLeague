/*
** EPITECH PROJECT, 2025
** GameTwo
** File description:
** configs.hpp
*/

#pragma once

    #include <string>
    #include <vector>

static const std::vector<std::string> PLUGIN_SYSTEMS {
    "poll_event",
    "movement2",
    "deal_damage",
    "kill_timeout",
    "follow_player",
    "animate",
    "draw",
    "display",
};

static const std::vector<std::string> CONFIG_PATHS {
    "assets/configs/tiles.toml",
    "assets/configs/player.toml",
    "assets/configs/xp.toml",
    "assets/configs/monsters/fire_skull.toml",
    "assets/configs/weapons/sword.toml",
    "assets/configs/weapons/bow.toml",
};

static const std::vector<std::string> MAP_PATHS {
    "assets/maps/basic.txt"
};
