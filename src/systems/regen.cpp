/*
** EPITECH PROJECT, 2026
** TekLeague
** File description:
** regen
*/

#include <clock.hpp>
#include <ECS/DenseZipper.hpp>
#include <ECS/Registry.hpp>

#include "components/stats/health.hpp"
#include "components/stats/mana.hpp"
#include "configs/systems.hpp"

void regeneration(Game &game) {
    game.createSystem("regeneration", [&game](ECS::Registry &) {
        static te::Timestamp delta(1.f);
        if (!delta.checkDelay())
            return;
        auto &healths = game.getComponent<Health>();
        auto &manas = game.getComponent<Mana>();

        for (auto&& [hp] : ECS::DenseZipper(healths)) {
            if (hp.amount < hp.max_amount) {
                hp.amount += hp.regen;
                if (hp.amount > hp.max_amount)
                    hp.amount = hp.max_amount;
            }
        }
        for (auto&& [mana] : ECS::DenseZipper(manas)) {
            if (mana.amount < mana.max_amount) {
                mana.amount += mana.regen;
                if (mana.amount > mana.max_amount)
                    mana.amount = mana.max_amount;
            }
        }
    }, false);
}
