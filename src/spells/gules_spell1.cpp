/*
** EPITECH PROJECT, 2025
** GameTwo
** File description:
** gules_spell1.hpp
*/

#include <physic/components/position.hpp>
#include "components/stats/mana.hpp"
#include "entity_spec/components/team.hpp"

#include "components/competences/spells.hpp"
#include "components/competences/target.hpp"
#include "components/gules.hpp"
#include "spells.hpp"

void gulesSpell1(Game& game, ECS::Entity e, const mat::Vector2f& mpos) {
    auto& positions = game.getComponent<addon::physic::Position2>();
    auto& teams = game.getComponent<addon::eSpec::Team>();
    auto& spells = game.getComponent<Spell>();
    auto& manas = game.getComponent<Mana>();
    auto& targets = game.getComponent<Target>();
    auto& gball = game.getComponent<Gball>();
    ECS::Entity gball_idx = 0;

    if (gball.hasComponent(e)) {
        gball_idx = gball.getComponent(e).idx;
        if (!gball.hasComponent(gball_idx)) {
            gball_idx = game.nextEntity(eType::PROJECTILES);
            game.createEntity(gball_idx, "gules_ball",
                positions.getComponent(e));
            gball.getComponent(e).idx = gball_idx;
            game.createComponent<Gball>(gball_idx, e);
            teams.getComponent(gball_idx).name = teams.getComponent(e).name;
            spells.getComponent(gball_idx).from = e;
            game.entities_queue.emplace_back(gball_idx, "gules_ball");
        } else {
            if (!spells.getComponent(gball_idx).cooldown.checkDelay())
                return;
        }
    } else {
        gball_idx = game.nextEntity(eType::PROJECTILES);
        game.createEntity(gball_idx, "gules_ball", positions.getComponent(e));
        game.createComponent<Gball>(e, gball_idx);
        game.createComponent<Gball>(gball_idx, e);
        teams.getComponent(gball_idx).name = teams.getComponent(e).name;
        spells.getComponent(gball_idx).from = e;
        game.entities_queue.emplace_back(gball_idx, "gules_ball");
    }

    auto& spell_info = spells.getComponent(gball_idx);
    auto& ball_pos = positions.getComponent(gball_idx);
    float dist =
        std::sqrt(std::pow(mpos.x - ball_pos.x, 2) + std::pow(mpos.y - ball_pos.y, 2));
    if ((dist > spell_info.range) ||
        manas.getComponent(e).amount < spell_info.mana_cost) {
        return;
    }
    spells.getComponent(gball_idx).arrived = false;
    targets.getComponent(gball_idx).x = mpos.x;
    targets.getComponent(gball_idx).y = mpos.y;
}
