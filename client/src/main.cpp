/*
** EPITECH PROJECT, 2025
** GameTwo
** File description:
** main.cpp
*/

#include "Game.hpp"
#include "scenes.hpp"

int main() {
    Game game;
    for (std::size_t i = 0; i < SCAST(SCENES::LIMITSCENE); ++i)
        SCENES_SETUPS.at(static_cast<SCENES>(i))(game);

    game.createEntity(game.nextEntity(eType::SYSTEM), "ig_window");
    game.activateScene(SCAST(SCENES::MAIN));
    game.run();
    return 0;
}
