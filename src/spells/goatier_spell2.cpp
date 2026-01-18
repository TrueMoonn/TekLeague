/*
** EPITECH PROJECT, 2025
** GameTwo
** File description:
** goatier_spell2.cpp
*/

#include <cmath>
#include <unordered_map>

#include <physic/components/position.hpp>

#include "components/competences/target.hpp"
#include "components/stats/mana.hpp"
#include "components/stats/stat_pool.hpp"
#include "components/competences/spells.hpp"
#include "spells.hpp"

constexpr float kDashRange = 600.f;
constexpr std::size_t kManaCost = 40;
constexpr float kCooldownSeconds = 8.f;

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

void goatierSpell2(Game& game, ECS::Entity caster, const mat::Vector2f& mpos)
{
    static std::unordered_map<ECS::Entity, te::Timestamp> cooldowns;

    auto& positions = game.getComponent<addon::physic::Position2>();
    auto& targets = game.getComponent<Target>();
    auto& manas = game.getComponent<Mana>();
    auto& stats = game.getComponent<StatPool>();

    if (!positions.hasComponent(caster) ||
        !targets.hasComponent(caster) ||
        !manas.hasComponent(caster) ||
        !stats.hasComponent(caster))
        return;

    auto [it, inserted] = cooldowns.try_emplace(caster, kCooldownSeconds);
    if (!inserted) {
        if (!it->second.checkDelay(false))
            return;
        it->second = te::Timestamp{kCooldownSeconds};
    }

    if (manas.getComponent(caster).amount < kManaCost)
        return;

    const auto& pos = positions.getComponent(caster);
    mat::Vector2f origin{pos.x, pos.y};
    mat::Vector2f dest = clampToRange(origin, mpos, kDashRange);

    positions.getComponent(caster).x = dest.x;
    positions.getComponent(caster).y = dest.y;

    auto& target = targets.getComponent(caster);
    target.x = dest.x;
    target.y = dest.y;
    target.to_attack = 0;

    manas.getComponent(caster).amount =
        manas.getComponent(caster).amount >= kManaCost ?
        manas.getComponent(caster).amount - kManaCost : 0;
    it->second.restart();
}
