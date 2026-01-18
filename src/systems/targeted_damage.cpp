/*
** EPITECH PROJECT, 2025
** GameTwo
** File description:
** targeted_damage.cpp
*/

#include <cmath>
#include <string>

#include <ECS/DenseZipper.hpp>
#include <physic/components/position.hpp>
#include <physic/components/hitbox.hpp>
#include <entity_spec/components/team.hpp>

#include "components/stats/health.hpp"
#include "components/competences/spells.hpp"
#include "components/competences/target.hpp"
#include "components/building.hpp"
#include "components/stats/stat_pool.hpp"
#include "configs/systems.hpp"
#include "maths/Rect.hpp"

static mat::RectF rectFrom(const addon::physic::Position2& pos,
    const addon::physic::Hitbox& hit)
{
    return mat::RectF(
        {pos.x + hit.position.x, pos.y + hit.position.y},
        {hit.size.x, hit.size.y});
}

static bool intersects(const mat::RectF& a, const mat::RectF& b)
{
    return a.position.x < b.position.x + b.size.x &&
        a.position.x + a.size.x > b.position.x &&
        a.position.y < b.position.y + b.size.y &&
        a.position.y + a.size.y > b.position.y;
}

static void handleNexusKill(Game& game, const Building& building,
    const addon::eSpec::Team& target_team)
{
    if (building.name == "nexus" && !game.getPendingWinningTeam().has_value()) {
        uint8_t winningTeam = target_team.name == "blue" ? 2 : 1;
        game.setPendingWinningTeam(winningTeam);
    }
}

void targetedDamage(Game& game) {
    game.createSystem("targeted_damage", [&game](ECS::Registry&) {
        auto& positions = game.getComponent<addon::physic::Position2>();
        auto& hitboxes = game.getComponent<addon::physic::Hitbox>();
        auto& spells = game.getComponent<Spell>();
        auto& targets = game.getComponent<Target>();
        auto& healths = game.getComponent<Health>();
        auto& teams = game.getComponent<addon::eSpec::Team>();
        auto& stats = game.getComponent<StatPool>();
        auto& buildings = game.getComponent<Building>();

        auto explodeAt = [&](ECS::Entity proj,
            const addon::physic::Position2& pos, Spell& spell,
            bool isAutoAttack, ECS::Entity ignored = 0) {
            if (spell.persistent && spell.arrived)
                return;
            if (!stats.hasComponent(spell.from) || !teams.hasComponent(proj))
                return;

            auto& caster_stats = stats.getComponent(spell.from);
            auto& projectile_team = teams.getComponent(proj);

            if (spell.aoe_radius > 0.0f) {
                for (auto&& [enemy_e, enemy_pos, enemy_health, enemy_team] :
                    ECS::IndexedDenseZipper(positions, healths, teams)) {
                    if (enemy_team.name == projectile_team.name)
                        continue;
                    if (enemy_e == ignored)
                        continue;
                    if (!isAutoAttack && buildings.hasComponent(enemy_e))
                        continue;
                    if (spells.hasComponent(enemy_e))
                        continue;

                    float dist_to_enemy = std::sqrt(
                        std::pow(enemy_pos.x - pos.x, 2) +
                        std::pow(enemy_pos.y - pos.y, 2));

                    if (dist_to_enemy <= spell.aoe_radius) {
                        enemy_health.reduceSafely(spell.base +
                            caster_stats.ad * spell.ratios[PHYSIC_DMG] +
                            caster_stats.ap * spell.ratios[MAGICAL_DMG]);
                        if (enemy_health.amount <= 0) {
                            if (buildings.hasComponent(enemy_e)) {
                                const auto& building = buildings.getComponent(enemy_e);
                                handleNexusKill(game, building, enemy_team);
                            }
                            game.AddKillEntity(enemy_e);
                        }
                    }
                }
            }

            spell.arrived = true;
            if (!spell.persistent) {
                game.AddKillEntity(proj);
            }
        };

        for (auto&& [e, pos, spell, tag] :
            ECS::IndexedDenseZipper(positions, spells, targets)) {

            const bool isAutoAttack = spell.name.size() >= 3 &&
                spell.name.rfind("_aa") == spell.name.size() - 3;

            // Collision-triggered AoE using hitboxes
            if (!spell.persistent && spell.aoe_radius > 0.0f &&
                teams.hasComponent(e)) {

                const auto& projectile_team = teams.getComponent(e);
                bool exploded = false;

                if (hitboxes.hasComponent(e)) {
                    const auto& proj_hit = hitboxes.getComponent(e);
                    mat::RectF proj_rect = rectFrom(pos, proj_hit);

                    for (auto&& [enemy_e, enemy_pos, enemy_team, enemy_hit] :
                        ECS::IndexedDenseZipper(positions, teams, hitboxes)) {
                        if (enemy_team.name == projectile_team.name)
                            continue;
                        if (!isAutoAttack && buildings.hasComponent(enemy_e))
                            continue;
                        if (spells.hasComponent(enemy_e))
                            continue;

                        mat::RectF enemy_rect = rectFrom(enemy_pos, enemy_hit);
                        if (intersects(proj_rect, enemy_rect)) {
                            explodeAt(e, pos, spell, isAutoAttack, enemy_e);
                            exploded = true;
                            break;
                        }
                    }
                }

                if (exploded)
                    continue;
            }

            // Cible spécifique
            if (tag.to_attack != 0) {
                if (!positions.hasComponent(tag.to_attack))
                    continue;
                const auto& target_pos = positions.getComponent(tag.to_attack);
                float dist = std::sqrt(std::pow(target_pos.x - pos.x, 2) +
                    std::pow(target_pos.y - pos.y, 2));
                if (dist < 30) {
                    if (spell.persistent && spell.arrived)
                        continue;
                    if (healths.hasComponent(tag.to_attack) &&
                        stats.hasComponent(spell.from)) {
                        if (!isAutoAttack && buildings.hasComponent(tag.to_attack))
                            continue;
                        if (spells.hasComponent(tag.to_attack))
                            continue;
                        auto& st = stats.getComponent(spell.from);
                        auto& target_health = healths.getComponent(tag.to_attack);
                        target_health.reduceSafely(spell.base +
                            st.ad * spell.ratios[PHYSIC_DMG] +
                            st.ap * spell.ratios[MAGICAL_DMG]);
                        if (target_health.amount <= 0) {
                            if (buildings.hasComponent(tag.to_attack)) {
                                const auto& building = buildings.getComponent(tag.to_attack);
                                if (teams.hasComponent(tag.to_attack)) {
                                    const auto& target_team =
                                        teams.getComponent(tag.to_attack);
                                    handleNexusKill(game, building, target_team);
                                }
                            }
                            game.AddKillEntity(tag.to_attack);
                        }
                    }
                    explodeAt(e, pos, spell, isAutoAttack, tag.to_attack);
                }
            }

            // AOE on destination (ground target)
            else {
                float dist_to_target = std::sqrt(std::pow(tag.x - pos.x, 2) +
                    std::pow(tag.y - pos.y, 2));

                if (dist_to_target < 30) {
                    explodeAt(e, pos, spell, isAutoAttack);
                }
            }
        }
    }, false);
}
