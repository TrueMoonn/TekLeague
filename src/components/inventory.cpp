/*
** EPITECH PROJECT, 2025
** GameTwo
** File description:
** item.cpp
*/

#include <ECS/Entity.hpp>

#include "components/inventory.hpp"

void registerInventory(Game& game) {
    game.registerComponent<Inventory>("inventory",
        [&game](ECS::Entity e, const toml::table& params) {
        std::array<std::size_t, MAX_ITEM_SLOT> arr;
        arr.fill(0);
        if (params["slots"].is_array()) {
            const auto& slots = params["slots"].as_array();
            for (std::size_t i = 0; i < MAX_ITEM_SLOT; ++i) {
                arr[i] = slots->at(i).value_or(0);
            }
        }
        game.createComponent<Inventory>(e, arr);
    });
}
