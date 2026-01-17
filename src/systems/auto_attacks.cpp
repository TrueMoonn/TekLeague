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
#include "components/competences/target.hpp"
#include "components/competences/spells.hpp"
#include "components/stats/health.hpp"
#include "components/stats/stat_pool.hpp"
#include "configs/systems.hpp"

void autoAttacks(Game &game) {
    game.createSystem("auto_attacks", [&game](ECS::Registry&) {
        auto& positions = game.getComponent<addon::physic::Position2>();
        auto& teams = game.getComponent<addon::eSpec::Team>();
        auto& spells = game.getComponent<Spell>();
        auto& spools = game.getComponent<StatPool>();
        auto& aas = game.getComponent<Attack>();
        auto& healths = game.getComponent<Health>();

        for (auto&& [e1, pos1, team1, aa, spool] :
            ECS::IndexedDenseZipper(positions, teams, aas, spools)) {
            for (auto&& [e2, pos2, team2, _] :
                ECS::IndexedDenseZipper(positions, teams, healths)) {
                if (!aa.automatic || team1.name == team2.name)
                    continue;
                if (sqrtf(pow(pos1.x - pos2.x, 2) +
                    pow(pos1.y - pos2.y, 2)) < spool.atk_range) {
                    if (spool.atk_speed.checkDelay()) {
                        std::string aname = aa.name;
                        ECS::Entity e = game.nextEntity(eType::PROJECTILES);
                        game.createEntity(e, aname, {pos1.x, pos1.y});
                        game.getComponent<Target>().
                            getComponent(e).to_attack = e2;
                        spells.getComponent(e).from = e1;
                        teams.getComponent(e).name = team1.name;
                        game.entities_queue.emplace_back(e, aname);
                    }
                }
            }
        };
    }, false);
}
