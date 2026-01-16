/*
** EPITECH PROJECT, 2026
** TekLeague
** File description:
** circular_collisions
*/


#include <cmath>

#include <ECS/DenseZipper.hpp>

#include <physic/components/hitbox.hpp>
#include <physic/components/movable.hpp>

#include <maths/Vector.hpp>
#include <physic/components/position.hpp>

#include "components/circle_hitbox.hpp"
#include "configs/systems.hpp"

#define S(val) std::pow(val, 2)

void circlePushback(Game &game) {
    game.createSystem("circle_pushback", [&game](ECS::Registry&) {
        auto &positions = game.getComponent<addon::physic::Position2>();
        auto &movable = game.getComponent<addon::physic::Movable>();
        auto &stationary = game.getComponent<addon::physic::Stationary>();
        auto &circleBoxes = game.getComponent<CircleHitbox>();

        for (auto &&[e1, pos1, c1, _]
            : ECS::IndexedDenseZipper(positions, circleBoxes, stationary)) {
            for (auto &&[e2, pos2, c2, _]
            : ECS::IndexedDenseZipper(positions, circleBoxes, movable)) {
                float dist = std::sqrt(S(pos2.x - pos1.x) + S(pos2.y - pos1.y));
                float mindist = c1.radius + c2.radius;
                if (S(dist) >= S(mindist) || e1 == e2)
                    continue;
                mat::Vector2f delta {pos2.x - pos1.x, pos2.y - pos1.y};
                mat::Vector2f disp = dist < 0.001f ? mat::Vector2f{1, 0} :
                    mat::Vector2f{delta.x / dist, delta.y / dist};
                pos2.x = pos1.x + disp.x * mindist;
                pos2.y = pos1.y + disp.y * mindist;
            }
        }
    });
}

/*

    // trap sur le sol ? du au midpoint trouvé

    game.createSystem("circle_pushback", [&game](ECS::Registry&) {
        auto &positions = game.getComponent<addon::physic::Position2>();
        auto &movable = game.getComponent<addon::physic::Movable>();
        auto &circleBoxes = game.getComponent<CircleHitbox>();

        for (auto &&[e1, pos1, c1] : ECS::IndexedDenseZipper(positions, circleBoxes)) {
            for (auto &&[e2, pos2, c2, _]
            : ECS::IndexedDenseZipper(positions, circleBoxes, movable)) {
                float dist = std::sqrt(S(pos2.x - pos1.x) + S(pos2.y - pos1.y));

                if (S(dist) >= S(c1.radius + c2.radius) || e1 == e2)
                    continue;
                mat::Vector2f midpoint ((pos1.x + pos2.x) / 2
                    , (pos1.y + pos2.y) / 2);
                pos2.x = midpoint.x + c2.radius * (pos2.x - pos1.x) / dist;
                pos2.y = midpoint.y + c2.radius * (pos2.y - pos1.y) / dist;
            }
        }
    });
*/
