/*
** EPITECH PROJECT, 2025
** GameTwo
** File description:
** targeted_damage.hpp
*/

#include <ECS/DenseZipper.hpp>
#include <physic/components/position.hpp>
#include <entity_spec/components/team.hpp>

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
        auto& teams = game.getComponent<addon::eSpec::Team>();

        auto& stats = game.getComponent<StatPool>();

        for (auto&& [e, pos, spell, tag] :
            ECS::IndexedDenseZipper(positions, spells, targets)) {

            // Cible spécifique
            if (tag.to_attack != 0) {
                if (!positions.hasComponent(tag.to_attack))
                    continue;
                const auto& target_pos = positions.getComponent(tag.to_attack);
                float dist = std::sqrt(std::pow(target_pos.x - pos.x, 2) +
                    std::pow(target_pos.y - pos.y, 2));
                if (dist < 10) {
                    if (spell.persistent && spell.arrived)
                        continue;
                    if (healths.hasComponent(tag.to_attack) &&
                        stats.hasComponent(spell.from)) {
                        auto& st = stats.getComponent(spell.from);
                        auto& target_health = healths.getComponent(tag.to_attack);
                        target_health.reduceSafely(spell.base +
                            st.ad * spell.ratios[PHYSIC_DMG] +
                            st.ap * spell.ratios[MAGICAL_DMG]);
                        if (target_health.amount <= 0) {
                            game.AddKillEntity(tag.to_attack);
                        }
                    }
                    spell.arrived = true;
                    if (!spell.persistent) {
                        game.AddKillEntity(e);
                    }
                }
            }

            // AOE
            else {
                float dist_to_target = std::sqrt(std::pow(tag.x - pos.x, 2) +
                    std::pow(tag.y - pos.y, 2));

                if (dist_to_target < 10) {
                    if (!stats.hasComponent(spell.from) ||
                        !teams.hasComponent(e))
                        continue;

                    if (spell.persistent && spell.arrived)
                        continue;

                    auto& caster_stats = stats.getComponent(spell.from);
                    auto& projectile_team = teams.getComponent(e);

                    if (spell.aoe_radius > 0.0f) {
                        for (auto&& [enemy_e, enemy_pos, enemy_health, enemy_team] :
                            ECS::IndexedDenseZipper(positions, healths, teams)) {
                            if (enemy_team.name == projectile_team.name)
                                continue;

                            float dist_to_enemy = std::sqrt(
                                std::pow(enemy_pos.x - pos.x, 2) +
                                std::pow(enemy_pos.y - pos.y, 2));

                            if (dist_to_enemy <= spell.aoe_radius) {
                                enemy_health.reduceSafely(spell.base +
                                    caster_stats.ad * spell.ratios[PHYSIC_DMG] +
                                    caster_stats.ap * spell.ratios[MAGICAL_DMG]);
                                if (enemy_health.amount <= 0) {
                                    game.AddKillEntity(enemy_e);
                                }
                            }
                        }
                    }

                    spell.arrived = true;

                    if (!spell.persistent) {
                        game.AddKillEntity(e);
                    }
                }
            }
        }
    });
}
