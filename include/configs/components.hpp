/*
** EPITECH PROJECT, 2025
** GameTwo
** File description:
** components.hpp
*/

#pragma once

    #include <vector>
    #include <functional>

    #include "Game.hpp"

    #include "components/champion.hpp"
    #include "components/stat_pool.hpp"
    #include "components/directed.hpp"

static const std::vector<std::function<void(Game&)>> LOCAL_COMPONENTS = {
    &registerChampion,
    &registerStatPool,
    &registerDirected,
};
