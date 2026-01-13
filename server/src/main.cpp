/*
** EPITECH PROJECT, 2025
** GameTwo
** File description:
** main.cpp
*/

#include <string>

#include "Server.hpp"
#include "scenes.hpp"

int main(int ac, char **av) {
    uint16_t port = 6767;
    std::string protocol = "UDP";

    if (ac > 1) {
        port = static_cast<uint16_t>(std::stoi(av[1]));
    }
    if (ac > 2) {
        protocol = av[2];
    }
    Server game(port, protocol);
    for (std::size_t i = 0; i < SCAST(SCENES::LIMITSCENE); ++i)
        SCENES_SETUPS.at(static_cast<SCENES>(i))(game);

    game.activateScene(SCAST(SCENES::WAIT));
    game.runServer();
    return 0;
}
