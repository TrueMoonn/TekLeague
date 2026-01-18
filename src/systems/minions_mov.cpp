/*
** EPITECH PROJECT, 2025
** GameTwo
** File description:
** kill_all_entity.hpp
*/

#include "ECS/Registry.hpp"
#include "components/competences/target.hpp"
#include "components/minions/minions.hpp"
#include "components/minions/minionsRoute.hpp"
#include "configs/systems.hpp"
#include "sfml/components/window.hpp"
#include <ECS/DenseZipper.hpp>
#include <physic/components/velocity.hpp>
#include <physic/components/position.hpp>
#include <display/components/animation.hpp>
#include <sfml/components/sprite.hpp>

void minionsMovement(Game &game) {
    game.createSystem("minion_movement", [&game](ECS::Registry&) {
       auto& minions = game.getComponent<Minions>();
       auto& posis = game.getComponent<addon::physic::Position2>();
       auto& pos_minions = game.getComponent<MinionsRoute>();
       auto& targets = game.getComponent<Target>();
       auto& vels = game.getComponent<addon::physic::Velocity2>();

       for (auto&& [e, minion, toStop, dir, pos, vel] :
           ECS::IndexedDenseZipper(minions, pos_minions, targets, posis, vels)) {
               if (std::abs(toStop.Positions.front().x - pos.x) < 20 &&
                   std::abs(toStop.Positions.front().y - pos.y) < 20)
                toStop.Positions.pop();
               if (toStop.Positions.empty() || toStop.Positions.front().x > 10000 || toStop.Positions.front().x < 0) {
                   // game.AddKillEntity(e);
                   // l'erreur est la, les minions envoie un projectile je sais pas pk et ca fait crash l'entièreté du jeu
                   return;
               }
               dir.x = toStop.Positions.front().x;
               dir.y = toStop.Positions.front().y;
           }
    }, false);
}
