/*
** EPITECH PROJECT, 2025
** GameTwo
** File description:
** goatier_spell1.cpp
*/

#include <cmath>
#include <unordered_map>

#include <physic/components/position.hpp>

#include "components/competences/spells.hpp"
#include "components/competences/target.hpp"
#include "components/stats/mana.hpp"
#include "entity_spec/components/team.hpp"
#include "spells.hpp"

static mat::Vector2f clampToRange(const mat::Vector2f& origin,
    const mat::Vector2f& destination, float max_range)
{
    mat::Vector2f dir = destination - origin;
    float len = std::sqrt(dir.x * dir.x + dir.y * dir.y);
    if (len <= max_range || len == 0.f)
        return destination;
    dir /= len;
    return origin + dir * max_range;
}

void goatierSpell1(Game& game, ECS::Entity caster, const mat::Vector2f& mpos)
{
    static std::unordered_map<ECS::Entity, te::Timestamp> cooldowns;

    auto& positions = game.getComponent<addon::physic::Position2>();
    auto& teams = game.getComponent<addon::eSpec::Team>();
    auto& spells = game.getComponent<Spell>();
    auto& targets = game.getComponent<Target>();
    auto& manas = game.getComponent<Mana>();

    if (!positions.hasComponent(caster) ||
        !teams.hasComponent(caster) ||
        !manas.hasComponent(caster))
        return;

    if (cooldowns.contains(caster) && !cooldowns.at(caster).checkDelay(false))
        return;

    ECS::Entity projectile = game.nextEntity(eType::PROJECTILES);
    game.createEntity(projectile, "goatier_spell1", positions.getComponent(caster));

    teams.getComponent(projectile).name = teams.getComponent(caster).name;
    spells.getComponent(projectile).from = caster;
    game.entities_queue.emplace_back(projectile, "goatier_spell1");

    auto& spell_info = spells.getComponent(projectile);
    auto [it, inserted] = cooldowns.try_emplace(caster, spell_info.cooldown);
    if (!inserted) {
        if (!it->second.checkDelay(false)) {
            game.AddKillEntity(projectile);
            return;
        }
        it->second = spell_info.cooldown;
    }

    if (manas.getComponent(caster).amount < spell_info.mana_cost) {
        game.AddKillEntity(projectile);
        return;
    }

    const auto& caster_pos = positions.getComponent(caster);
    mat::Vector2f dest = clampToRange(
        {caster_pos.x, caster_pos.y},
        mpos,
        static_cast<float>(spell_info.range));

    auto& target = targets.getComponent(projectile);
    target.x = dest.x;
    target.y = dest.y;
    target.to_attack = 0;
    spell_info.arrived = false;
    it->second.restart();
}
