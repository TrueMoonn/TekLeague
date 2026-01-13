/*
** EPITECH PROJECT, 2025
** GameTwo
** File description:
** track_target.hpp
*/

#include <ECS/DenseZipper.hpp>
#include <cmath>
#include <physic/components/velocity.hpp>
#include <physic/components/position.hpp>
#include <display/components/animation.hpp>
#include <sfml/components/sprite.hpp>

#include "ECS/DenseSA.hpp"
#include "ECS/Registry.hpp"
#include "components/auto_track.hpp"
#include "configs/systems.hpp"
#include "maths/Vector.hpp"

void trackTarget(Game& game) {
    game.createSystem("track_target", [&game](ECS::Registry&){
        auto& autoTrack = game.getComponent<AutoTrack>();
        auto& velocities = game.getComponent<addon::physic::Velocity2>();
        auto& positions = game.getComponent<addon::physic::Position2>();

        for (auto&& [track, vel, pos] : ECS::DenseZipper(autoTrack, velocities, positions)) {
            if (track.toFollow == 0 || !track.track || !positions.hasComponent(track.toFollow)) continue;
            auto& Tpos = GET_ENTITY_CMPT(positions, track.toFollow);
            vel.x = Tpos.x - pos.x;
            vel.y = Tpos.y - pos.y;
            vel.normalize();
            float dist = vel.distance(pos);
            if (dist < 150) {
                mat::Vector2f dir(pos - Tpos);
                float length = vel.length();
                dir /= length;
                vel.x += dir.x * dist;
                vel.y += dir.y * dist;
            }
            vel *= track.vel;
        }
    });
}
