/*
** EPITECH PROJECT, 2025
** GameTwo
** File description:
** gold.hpp
*/

#pragma once

    #include <GameTool.hpp>

struct Gold {
    std::size_t amount;
};

void registerGold(te::GameTool& game);
