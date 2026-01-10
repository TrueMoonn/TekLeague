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

    game.activateScene(SCAST(SCENES::MENU));
    game.run();
    return 0;
}
