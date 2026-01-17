/*
** EPITECH PROJECT, 2025
** GameTwo
** File description:
** health_bar.cpp
*/

#include "components/ui/game/sticky.hpp"
#include "ECS/Entity.hpp"
#include <toml++/impl/table.hpp>

void registerSticky(Game &game) {
    game.registerComponent<Sticky>("sticky",
        [&game](ECS::Entity e, const toml::table &params) {
        const auto &ofArr = params["offset"].as_array();
        mat::Vector2i offset = ofArr && ofArr->size() == 2 ? mat::Vector2i{
            ofArr->at(0).value_or<int>(0), ofArr->at(1).value_or<int>(0)
        } : mat::Vector2i{0, 0};
        game.createComponent<Sticky>(e, offset.x, offset.y);
    });
}
