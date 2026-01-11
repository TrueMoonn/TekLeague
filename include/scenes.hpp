/*
** EPITECH PROJECT, 2026
** GameTwo
** File description:
** scenes.hpp
*/

#pragma once

    #include <functional>
    #include <unordered_map>

    #include "Game.hpp"

    #define SCAST(s) static_cast<std::size_t>(s)

void setClientScene(Game& game);
void setInGameScene(Game& game);

enum class SCENES {
    CLIENT = 0,
    INGAME,
    LIMITSCENE,
};

static const std::unordered_map<SCENES,
    std::function<void(Game&)>> SCENES_SETUPS = {
    {SCENES::CLIENT, &setClientScene},
    {SCENES::INGAME, &setInGameScene},
};
