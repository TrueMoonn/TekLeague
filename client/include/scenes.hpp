/*
** EPITECH PROJECT, 2026
** GameTwo
** File description:
** scenes.hpp
*/

#pragma once

    #include <functional>
    #include <unordered_map>

    #include "Client.hpp"

    #define SCAST(s) static_cast<std::size_t>(s)

void setMainScene(Client& game);
void setInGameScene(Client& game);

enum class SCENES {
    MAIN = 0,
    INGAME,
    LIMITSCENE,
};

static const std::unordered_map<SCENES,
    std::function<void(Client&)>> SCENES_SETUPS = {
    {SCENES::MAIN, &setMainScene},
    {SCENES::INGAME, &setInGameScene},
};
