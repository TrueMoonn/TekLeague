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

#include "components/auto_track.hpp"
    #include "components/champion.hpp"
    #include "components/stat_pool.hpp"
    #include "components/button.hpp"
    #include "components/inventory.hpp"
    #include "components/item.hpp"
    #include "components/target.hpp"
    #include "components/ranged_zone.hpp"

static const std::vector<std::function<void(Game&)>> LOCAL_COMPONENTS = {
    &registerChampion,
    &registerStatPool,
    &registerButton,
    &registerInventory,
    &registerItem,
    &registerTarget,
    &registerRangedZone,
    &registerAutoTrack,
};
