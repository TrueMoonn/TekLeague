/*
** EPITECH PROJECT, 2025
** GameTwo
** File description:
** button.cpp
*/

#include <ECS/Entity.hpp>

#include "components/ui/button.hpp"

void registerButton(te::GameTool& game) {
    game.registerComponent<Button>("button",
        [&game](ECS::Entity e, const toml::table &) {
        game.createComponent<Button>(e);
    });
}
