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
void setLobbyScene(Client& game);
void setSearchLobbyScene(Client& game);
void setInLobbyScene(Client& game);
void setParametersScenes(Client& game);
void setInGameScene(Client& game);
void setChampionSelection(Client& game);

enum class SCENES {
    MAIN = 0,
    LOBBY,
    SEARCH_LOBBY,
    IN_LOBBY,
    SELECT_CHAMP,
    PARAMETERS,
    INGAME,
    LIMITSCENE,
};

static const std::unordered_map<SCENES,
    std::function<void(Client&)>> SCENES_SETUPS = {
    {SCENES::MAIN, &setMainScene},
    {SCENES::LOBBY, &setLobbyScene},
    {SCENES::SEARCH_LOBBY, &setSearchLobbyScene},
    {SCENES::IN_LOBBY, &setInLobbyScene},
    {SCENES::SELECT_CHAMP, &setChampionSelection},
    {SCENES::PARAMETERS, &setParametersScenes},
    {SCENES::INGAME, &setInGameScene},
};
