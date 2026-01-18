/*
** EPITECH PROJECT, 2025
** GameTwo
** File description:
** spells.hpp
*/

#pragma once

    #include <string>
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

struct Spell {
    Spell(const std::string& name, std::size_t base, EffectType dtype,
        const std::array<std::size_t, DamageType::DMGLIMIT>& ratio,
        float cd, float cast, SpellTarget to, std::size_t mc,
        std::size_t spd, std::size_t r, float aoe = 0.0f, bool persist = false) :
        name(name), base(base), dmgType(dtype), ratios(ratio), cooldown(cd),
        castTime(cast), target(to), mana_cost(mc), spellSpeed(spd), range(r),
        aoe_radius(aoe), persistent(persist) {}

    std::string name;
    ECS::Entity from = 0;

    std::size_t base;
    EffectType dmgType;
    std::array<std::size_t, DamageType::DMGLIMIT> ratios;
    std::size_t mana_cost;

    te::Timestamp cooldown{0.f};
    te::Timestamp castTime{0.f};

    std::size_t spellSpeed;
    std::size_t range;
    SpellTarget target;
    float aoe_radius;
    bool persistent;
    bool arrived = false;
};

struct Spells {
    explicit Spells(const std::vector<std::size_t>& spellid) :
        spell_id(spellid) {}
    std::vector<std::size_t> spell_id;
};

void registerSpellCasting(Game& game);
