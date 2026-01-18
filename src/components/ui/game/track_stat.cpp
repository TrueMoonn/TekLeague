/*
** EPITECH PROJECT, 2025
** GameTwo
** File description:
** track_stat.cpp
*/

#include "components/ui/game/track_stat.hpp"
#include "ECS/Entity.hpp"
#include <toml++/impl/table.hpp>

void registerTrackStat(Game &game) {
    game.registerComponent<TrackStat>("track_stat",
        [&game](ECS::Entity e, const toml::table &params) {
            std::string toFollow = params["stat"].value_or("");
            game.createComponent<TrackStat>(e, toFollow);
        });
}
