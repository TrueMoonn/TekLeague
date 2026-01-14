/*
** EPITECH PROJECT, 2025
** GameTwo
** File description:
** main.cpp
*/

#include "Client.hpp"
#include "scenes.hpp"

int main() {
    Client game;
    for (std::size_t i = 0; i < SCAST(SCENES::LIMITSCENE); ++i)
        SCENES_SETUPS.at(static_cast<SCENES>(i))(game);

    game.createEntity(game.nextEntity(eType::SYSTEM), "ig_window");
    game.updateScene(te::sStatus::ACTIVATE, SCAST(SCENES::MAIN));
    game.run();
    return 0;
}
