/*
** EPITECH PROJECT, 2025
** GameTwo
** File description:
** gules_spell2.cpp
*/

#include <physic/components/position.hpp>

#include "components/competences/spells.hpp"
#include "components/competences/target.hpp"
#include "components/gules.hpp"
#include "components/stats/mana.hpp"
#include "entity_spec/components/team.hpp"

void gulesSpell2(Game& game, ECS::Entity e, const mat::Vector2f&) {
    auto& gball = game.getComponent<Gball>();
    if (!gball.hasComponent(e))
        return;

    ECS::Entity gball_idx = gball.getComponent(e).idx;

    auto& positions = game.getComponent<addon::physic::Position2>();
    auto& spells = game.getComponent<Spell>();
    auto& targets = game.getComponent<Target>();
    auto& manas = game.getComponent<Mana>();

    if (!spells.hasComponent(gball_idx) ||
        !targets.hasComponent(gball_idx) ||
        !positions.hasComponent(e) ||
        !manas.hasComponent(e))
        return;

    auto& spell_info = spells.getComponent(gball_idx);

    if (!spell_info.cooldown.checkDelay())
        return;
    auto& caster_mana = manas.getComponent(e);
    if (caster_mana.amount < static_cast<int>(spell_info.mana_cost))
        return;

    caster_mana.amount -= static_cast<int>(spell_info.mana_cost);
    if (caster_mana.amount < 0)
        caster_mana.amount = 0;

    const auto& caster_pos = positions.getComponent(e);
    targets.getComponent(gball_idx).x = caster_pos.x;
    targets.getComponent(gball_idx).y = caster_pos.y;

    spell_info.arrived = false;
    spell_info.from = e;
}
