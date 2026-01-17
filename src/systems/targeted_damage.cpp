/*
** EPITECH PROJECT, 2025
** GameTwo
** File description:
** targeted_damage.hpp
*/

#include <ECS/DenseZipper.hpp>
#include <physic/components/position.hpp>

#include "components/stats/health.hpp"
#include "components/competences/spells.hpp"
#include "components/competences/target.hpp"

#include "components/stats/stat_pool.hpp"
#include "configs/systems.hpp"

void targetedDamage(Game& game) {
    game.createSystem("targeted_damage", [&game](ECS::Registry&) {
        auto& positions = game.getComponent<addon::physic::Position2>();
        auto& spells = game.getComponent<Spell>();
        auto& targets = game.getComponent<Target>();
        auto& healths = game.getComponent<Health>();

        auto& stats = game.getComponent<StatPool>();

        for (auto&& [e, pos, spell, tag] :
            ECS::IndexedDenseZipper(positions, spells, targets)) {
            if (tag.to_attack == 0)
                continue;
            const auto& target_pos = positions.getComponent(tag.to_attack);
            float dist = std::sqrt(std::pow(target_pos.x - pos.x, 2) +
                std::pow(target_pos.y - pos.y, 2));
            if (dist < 10) {
                if (healths.hasComponent(tag.to_attack) &&
                    stats.hasComponent(spell.from)) {
                    auto& st = stats.getComponent(spell.from);
                    auto& target_health = healths.getComponent(tag.to_attack);
                    target_health.reduceSafely(spell.base +
                        st.ad * spell.ratios[PHYSIC_DMG] +
                        st.ap * spell.ratios[MAGICAL_DMG]);
                }
                game.AddKillEntity(e);
            }
        }
    });
}
