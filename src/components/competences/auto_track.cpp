/*
** EPITECH PROJECT, 2025
** GameTwo
** File description:
** auto_track.cpp
*/

#include "components/competences/auto_track.hpp"

void registerAutoTrack(Game &game) {
    game.registerComponent<AutoTrack>("auto_track",
        [&game](ECS::Entity e, const toml::table &params) {
            int speed = params["speed"].value_or(0);
            game.createComponent<AutoTrack>(e, speed, true);
    });
}
