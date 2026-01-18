/*
** EPITECH PROJECT, 2025
** GameTwo
** File description:
** update_sticky.hpp
*/

#include <ECS/DenseZipper.hpp>
#include <physic/components/velocity.hpp>
#include <physic/components/position.hpp>
#include <display/components/animation.hpp>
#include <sfml/components/sprite.hpp>

#include "ECS/Registry.hpp"
#include "components/ui/game/sticky.hpp"
#include "client_systems.hpp"

void updateSticky(Game& game) {
    game.createSystem("update_sticky", [&game](ECS::Registry&){
        auto& stickys = game.getComponent<Sticky>();
        auto& positions = game.getComponent<addon::physic::Position2>();

        for (ECS::Entity e = 0; e < ONDEATH_END; ++e) {
            if (!stickys.hasComponent(e) || !positions.hasComponent(e))
                continue;
            
            auto& sticky = stickys.getComponent(e);
            auto& position = positions.getComponent(e);
            
            if (sticky.id == 0 || !positions.hasComponent(sticky.id))
                continue;
            auto &pos = positions.getComponent(sticky.id);
            position.x = pos.x + sticky.x;
            position.y = pos.y + sticky.y;
        }
    }, false);
}
