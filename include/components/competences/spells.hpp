/*
** EPITECH PROJECT, 2025
** GameTwo
** File description:
** spells.hpp
*/

#pragma once

    #include <array>
    #include <vector>

    #include <clock.hpp>
    #include <maths/Vector.hpp>

#include "ECS/Entity.hpp"
    #include "Game.hpp"

enum DamageType : std::size_t {
    PHYSIC_DMG = 0,
    MAGICAL_DMG,
    DMGLIMIT,
};

enum EffectType : std::size_t  {
    PHYSIC_EFF = 0,
    MAGICAL_EFF,
    SHIELD_EFF,
    HEAL_EFF,
    EFFECTLIMIT,
};

enum SpellTarget : std::size_t {
    ANY = 0,
    ALLY,
    ENEMY,
    MONSTER,
};

struct SpellEntities {
    std::string name;
    mat::Vector2f pos;
};

struct Casting {
    Casting(const std::string& name, std::size_t base, EffectType dtype,
        const std::array<std::size_t, DamageType::DMGLIMIT>& ratio,
        float cd, float cast, SpellTarget to,
        const std::vector<SpellEntities>& es) :
        name(name), base(base), dmgType(dtype), ratios(ratio), cooldown(cd),
        castTime(cast), target(to), entities(es) {}

    std::string name;

    std::size_t base;
    EffectType dmgType;
    std::array<std::size_t, DamageType::DMGLIMIT> ratios;

    te::Timestamp cooldown;
    te::Timestamp castTime;

    SpellTarget target;
    std::vector<SpellEntities> entities;
};

struct Spells {
    Spells(const std::vector<Casting>& sps) : spells(sps) {}
    std::vector<Casting> spells;
};

struct SpellEntity {
    SpellEntity(ECS::Entity e, int spell) : entity(e), spellIdx(spell) {};
    ECS::Entity entity;
    int spellIdx;
};

void registerSpellCasting(Game& game);
