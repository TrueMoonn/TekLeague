/*
** EPITECH PROJECT, 2026
** GameTwo
** File description:
** systems.cpp
*/

#pragma once

    #include <string>
    #include <vector>

static const std::vector<std::string> SERVER_SYSTEMS = {
    "kill_all_entity",
    "auto_attacks",

    // PRE MOVEMENT
    "track_target",
    "minion_movement",

    // MOVEMENT
    "movement2",
    "circle_pushback",

    // STATS UPDATE
    "targeted_damage",
    "fountain",
    "regeneration",

    // POST STATS UPDATE
    "handle_timeout",
    "spawn_death_entity",
};
