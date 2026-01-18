/*
** EPITECH PROJECT, 2025
** GameTwo
** File description:
** spells.hpp
*/

#pragma once

    #include <unordered_map>
    #include <functional>

    #include <maths/Vector.hpp>

    #include "Game.hpp"

enum class SpellId {
    NONE = 0,
    GULES_1,
    GULES_2,
    GOATIER_1,
    GOATIER_2,
};

void noSpell(Game&, ECS::Entity, const mat::Vector2f&);
void gulesSpell1(Game&, ECS::Entity, const mat::Vector2f&);
void gulesSpell2(Game&, ECS::Entity, const mat::Vector2f&);
void goatierSpell1(Game&, ECS::Entity, const mat::Vector2f&);
void goatierSpell2(Game&, ECS::Entity, const mat::Vector2f&);

static const std::unordered_map<SpellId,
    std::function<void(Game&, ECS::Entity, const mat::Vector2f&)>> SPELLS = {
    {SpellId::NONE, &noSpell},
    {SpellId::GULES_1, &gulesSpell1},
    {SpellId::GULES_2, &gulesSpell2},
    {SpellId::GOATIER_1, &goatierSpell1},
    {SpellId::GOATIER_2, &goatierSpell2},
};
