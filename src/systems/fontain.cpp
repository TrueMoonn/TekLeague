/*
** EPITECH PROJECT, 2026
** TekLeague
** File description:
** fontain.cpp
*/

#include <clock.hpp>
#include <ECS/DenseZipper.hpp>
#include <ECS/Registry.hpp>
#include <physic/components/position.hpp>

#include "ECS/Entity.hpp"
#include "components/building.hpp"
#include "components/champion.hpp"
#include "components/stats/health.hpp"
#include "components/stats/mana.hpp"
#include "configs/systems.hpp"
#include "entity_spec/components/team.hpp"

void fountainInteractions(Game &game) {
    game.createSystem("fountain", [&game](ECS::Registry &) {
        static te::Timestamp delta(0.2f);
        if (!delta.checkDelay())
            return;
        auto& posis = game.getComponent<addon::physic::Position2>();
        auto& teams = game.getComponent<addon::eSpec::Team>();
        auto& builds = game.getComponent<Building>();
        auto& champs = game.getComponent<Champion>();
        auto& healts = game.getComponent<Health>();
        auto& manas = game.getComponent<Mana>();

        for (auto&& [build, bteam, bpos] : ECS::DenseZipper(builds, teams, posis)) {
            if (build.name != "fontain")
                continue;
            for (auto&& [e, champ, hp, cteam, cpos]:
                ECS::IndexedDenseZipper(champs, healts, teams, posis)) {
                float dist = std::sqrt(
                    std::pow(cpos.x - bpos.x, 2) +
                    std::pow(cpos.y - bpos.y, 2));
                if (dist > 500)
                    continue;
                if (cteam.name == bteam.name) {
                    if (hp.amount < hp.max_amount) {
                        hp.amount += hp.max_amount * 15.f / 100.f;
                        if (hp.amount > hp.max_amount)
                            hp.amount = hp.max_amount;
                    }
                    if (manas.hasComponent(e)) {
                        auto& mana = manas.getComponent(e);
                        if (mana.amount >= mana.max_amount)
                            continue;
                        mana.amount += mana.max_amount * 15.f / 100.f;
                        if (mana.amount > mana.max_amount)
                            mana.amount = mana.max_amount;
                    }
                } else if (cteam.name != bteam.name) {
                    hp.reduceSafely(2000);
                }
            }
        }
    }, false);
}
