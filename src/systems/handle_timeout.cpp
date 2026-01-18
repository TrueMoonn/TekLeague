/*
** EPITECH PROJECT, 2026
** TekLeague
** File description:
** handle_timeout
*/

#include <ECS/DenseZipper.hpp>
#include <ECS/Registry.hpp>

#include "entity_spec/components/timeout.hpp"

#include "configs/systems.hpp"

void handleTimeout(Game &game)
{
    game.createSystem("handle_timeout", [&game](ECS::Registry &) {
        auto &timeouts = game.getComponent<addon::eSpec::Timeout>();

        for (auto &&[e, to] : ECS::IndexedDenseZipper(timeouts)) {
            if (to.delta.checkDelay(false))
                game.AddKillEntity(e);
        }
    });
}
