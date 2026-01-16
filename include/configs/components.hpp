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
    #include "components/on_death.hpp"
    #include "components/competences/auto_attack.hpp"
    #include "components/competences/auto_track.hpp"
    #include "components/competences/target.hpp"
    #include "components/items/inventory.hpp"
    #include "components/items/item.hpp"
    #include "components/stats/gold.hpp"
    #include "components/stats/health.hpp"
    #include "components/stats/mana.hpp"
    #include "components/stats/stat_pool.hpp"
    #include "components/stats/xp.hpp"
    #include "components/ui/button.hpp"

static const std::vector<std::function<void(Game&)>> LOCAL_COMPONENTS = {
    &registerAutoAttack,
    &registerAutoTrack,
    &registerOnDeath,
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
