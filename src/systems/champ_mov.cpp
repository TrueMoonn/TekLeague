/*
** EPITECH PROJECT, 2025
** GameTwo
** File description:
** looking_forward.hpp
*/

#include <cmath>

#include <ECS/DenseZipper.hpp>
#include <physic/components/velocity.hpp>
#include <display/components/animation.hpp>
#include <sfml/components/sprite.hpp>

#include "components/champion.hpp"
#include "configs/systems.hpp"

void championMovement(Game &game) {
    game.createSystem("champion_movement", [&game](ECS::Registry&){
        auto& vels = game.getComponent<addon::physic::Velocity2>();
        auto& sprites = game.getComponent<addon::sfml::Sprite>();
        auto& anims = game.getComponent<addon::display::Animation>();
        auto& champs = game.getComponent<Champion>();

        for (auto&& [_, vel, sp, anim] :
            ECS::DenseZipper(champs, vels, sprites, anims)) {
            int cframe = 0;
            if (vel.x != 0 && vel.y != 0)
                cframe = 5;
            auto scale = sp.sp.getScale();
            if (vel.x > 0 && scale.x > 0) {
                scale.x *= -1;
                sp.sp.setScale(scale);
            } else if (vel.x < 0 && scale.x <= 0) {
                scale.x *= -1;
                sp.sp.setScale(scale);
            }
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
            if (anim.curAnim != cframe) {
                anim.curAnim = cframe;
                anim.timestamp.delay = anim.frameInfos[cframe].frameDELAY;
            }
        }
    });
}
