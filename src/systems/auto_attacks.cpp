/*
** EPITECH PROJECT, 2025
** GameTwo
** File description:
** target_player.hpp
*/

#include <ECS/DenseZipper.hpp>
#include <cmath>
#include <physic/components/velocity.hpp>
#include <physic/components/position.hpp>
#include <display/components/animation.hpp>
#include <entity_spec/components/team.hpp>
#include <sfml/components/sprite.hpp>

#include "components/competences/auto_attack.hpp"
#include "components/competences/auto_track.hpp"
#include "components/stats/stat_pool.hpp"
#include "configs/systems.hpp"

static void shootAt(Game &game, const addon::physic::Position2 &towerPos,
    const addon::physic::Position2 &playerPos, ECS::Entity Player)
{
    auto e = game.nextEntity(eType::PROJECTILES);
    game.createEntity(e, "tower_projectiles", towerPos);
    auto& track = game.getComponent<AutoTrack>().getComponent(e);
    track.toFollow = Player;
}

void autoAttacks(Game &game) {
    game.createSystem("auto_attacks", [&game](ECS::Registry&) {
        auto& positions = game.getComponent<addon::physic::Position2>();
        auto& teams = game.getComponent<addon::eSpec::Team>();
        auto& stats = game.getComponent<StatPool>();
        auto& aa = game.getComponent<AutoAttack>();

        for (auto&& [e1, pos1, stat1, team1, _] :
            ECS::IndexedDenseZipper(positions, stats, teams, aa)) {
            if (!stat1.atk_speed.checkDelay())
                continue;
            for (auto&& [e2, pos2, stat2, team2] :
                ECS::IndexedDenseZipper(positions, stats, teams)) {
                if (!team1.name.compare(team2.name))
                    continue;
                if (sqrtf(pow(pos1.x - pos2.x, 2) +
                    pow(pos1.y - pos2.y, 2)) < stat1.atk_range) {
                    if (stat1.atk_speed.checkDelay())
                        shootAt(game, pos1, pos2, e2);
                }
            }
        };
    });
}
