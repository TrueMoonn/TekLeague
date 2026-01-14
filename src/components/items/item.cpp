/*
** EPITECH PROJECT, 2025
** GameTwo
** File description:
** item.cpp
*/

#include <ECS/Entity.hpp>

#include "components/items/item.hpp"

void registerItem(Game& game) {
    game.registerComponent<Item>("item",
        [&game](ECS::Entity e, const toml::table& params) {
        std::size_t id = params["id"].value_or(0);
        game.createComponent<Item>(e, id);
    });
}
