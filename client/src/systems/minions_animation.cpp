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
#include <ECS/DenseZipper.hpp>
#include <physic/components/velocity.hpp>
#include <physic/components/position.hpp>
#include <display/components/animation.hpp>
#include <sfml/components/sprite.hpp>

void minionsMovement(Game &game) {
game.createSystem("minion_animation", [&game](ECS::Registry&) {
    auto& minions = game.getComponent<Minions>();
    auto& targets = game.getComponent<Target>();
    auto& sprites = game.getComponent<addon::sfml::Sprite>();
    auto& animations = game.getComponent<addon::display::Animation>();
    auto& vels = game.getComponent<addon::physic::Velocity2>();

    for (auto&& [minion, sp, anim, vel, _] :
        ECS::DenseZipper(minions, sprites, animations, vels, targets)) {
        std::size_t cframe =
            static_cast<std::size_t>(MinionState::IDLE);
            if (minion.state == MinionState::IDLE && (vel.x != 0 || vel.y != 0))
                minion.state = MinionState::MOVEMENT;
            if (minion.state == MinionState::MOVEMENT) {
                if (vel.x == 0 && vel.y == 0) {
                    minion.state = MinionState::IDLE;
                    if (anim.curAnim != cframe) {
                        anim.curAnim = cframe;
                        anim.timestamp.delay = anim.frameInfos[cframe].frameDELAY;
                    }
                    continue;
                }
            cframe = static_cast<std::size_t>(MinionState::MOVEMENT);
            float angle = std::atan2(vel.y, vel.x) / M_PI * 180.f;
            if (angle < 0) angle += 360.f;
            if ((angle >= 337.5f || angle < 22.5f) ||
            (angle >= 157.5f && angle < 202.5f)) {
                cframe += 2;
            } else if (angle >= 22.5f && angle < 67.5f ||
            angle >= 112.5f && angle < 157.5f) {
                cframe += 1;
            } else if (angle >= 202.5f && angle < 247.5f ||
            angle >= 292.5f && angle < 337.5f) {
                cframe += 3;
            } else if (angle >= 247.5f && angle < 292.5f) {
            cframe += 4;
            }
            auto scale = sp.sp.getScale();
            if (vel.x > 0 && scale.x > 0) {
                scale.x *= -1;
                sp.sp.setScale(scale);
            } else if (vel.x < 0 && scale.x <= 0) {
                scale.x *= -1;
                sp.sp.setScale(scale);
            }
        }
        if (anim.curAnim != cframe) {
            anim.curAnim = cframe;
            anim.timestamp.delay = anim.frameInfos[cframe].frameDELAY;
        }
    }
    }, false);
}
