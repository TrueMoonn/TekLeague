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

    #include "components/auto_attack.hpp"
    #include "components/auto_track.hpp"
    #include "components/button.hpp"
    #include "components/champion.hpp"
    #include "components/gold.hpp"
    #include "components/health.hpp"
    #include "components/inventory.hpp"
    #include "components/item.hpp"
    #include "components/mana.hpp"
    #include "components/stat_pool.hpp"
    #include "components/target.hpp"
    #include "components/xp.hpp"

static const std::vector<std::function<void(Game&)>> LOCAL_COMPONENTS = {
    &registerAutoAttack,
    &registerAutoTrack,
    &registerButton,
    &registerChampion,
    &registerGold,
    &registerHealth,
    &registerInventory,
    &registerItem,
    &registerMana,
    &registerStatPool,
    &registerTarget,
    &registerXp,
};
