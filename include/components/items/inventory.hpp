/*
** EPITECH PROJECT, 2025
** GameTwo
** File description:
** inventory.hpp
*/

#pragma once

    #include <array>

    #include "Game.hpp"

    #define MAX_ITEM_SLOT 6

struct Inventory {
    Inventory(const std::array<std::size_t, MAX_ITEM_SLOT>& arr) :
        slots(arr) {}

    std::array<std::size_t, MAX_ITEM_SLOT> slots;
};

void registerInventory(Game& game);
