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
#include <sfml/components/sprite.hpp>

#include "ECS/DenseSA.hpp"
#include "ECS/Entity.hpp"
#include "ECS/Registry.hpp"
#include "components/auto_track.hpp"
#include "components/target.hpp"
#include "components/champion.hpp"
#include "components/ranged_zone.hpp"
#include "components/stat_pool.hpp"
#include "configs/systems.hpp"
#include "maths/Vector.hpp"

static void shootAtPlayer(Game &game, const addon::physic::Position2 &towerPos,
    const addon::physic::Position2 &playerPos, ECS::Entity Player)
{
    auto e = game.nextEntity(eType::PROJECTILES);
    game.createEntity(e, "tower_projectiles", towerPos);
    auto& track = GET_ENTITY_CMPT(game.getComponent<AutoTrack>(), e);

    track.toFollow = Player;
}

void targetPlayer(Game &game) {
    game.createSystem("target_player", [&game](ECS::Registry&) {
        auto& positions = game.getComponent<addon::physic::Position2>();
        auto& sprites = game.getComponent<addon::sfml::Sprite>();
        auto& champs = game.getComponent<Champion>();
        auto& zones = game.getComponent<RangedZone>();

        for (auto&& [posZ, zoneZ, spZ] :
            ECS::DenseZipper(positions, zones, sprites)) {
                for (auto&& [e, posC, _, spC] :
                    ECS::IndexedDenseZipper(positions, champs, sprites)) {
                        if (sqrtf(pow(posZ.x - posC.x, 2) + pow(posZ.y - posC.y, 2)) < zoneZ.radius) {
                            if (zoneZ.delay.checkDelay())
                                shootAtPlayer(game, posZ, posC, e);
                        }
                        else
                            zoneZ.delay.restart();
                    }
            };
    });
}
