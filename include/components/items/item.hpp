/*
** EPITECH PROJECT, 2025
** GameTwo
** File description:
** item.hpp
*/

#pragma once

    #include <vector>
    #include <string>

    #include <GameTool.hpp>

static const std::vector<std::string> ITEMS = {
    "none",
    "Casquette Musclée",
    "BF Sac",
    "Pantalon Solide",
    "Claquettes Démoniaques",
    "Cheveulure Soyeuse",
};

struct Item {
    Item(std::size_t item_id) : id(item_id) {}

    std::size_t id;
};

void registerItem(te::GameTool& game);
