/*
** EPITECH PROJECT, 2025
** GameTwo
** File description:
** deal_damage.cpp
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
#include "components/champion.hpp"
#include "components/competences/auto_track.hpp"
#include "components/stats/health.hpp"
#include "components/stats/stat_pool.hpp"
#include "configs/systems.hpp"
#include "entity_spec/components/team.hpp"
#include "maths/Rect.hpp"
#include "physic/components/hitbox.hpp"

static bool square_hitbox(mat::RectF rect1, mat::RectF rect2) {
    return (rect1.position.x < rect2.position.x + rect2.size.x &&
        rect1.position.x + rect1.size.x > rect2.position.x &&
        rect1.position.y < rect2.position.y + rect2.size.y &&
        rect1.position.y + rect1.size.y > rect2.position.y);
}

static mat::RectF true_hitbox(const addon::physic::Position2 &pos,
    const addon::physic::Hitbox &hit) {
    return mat::RectF(
        mat::Vector2f(pos.x + hit.position.x, pos.y + hit.position.y),
        mat::Vector2f(hit.size.x, hit.size.y));
}

static std::vector<ECS::Entity> entity_hit_team(Game& reg,
    const ECS::Entity& entity) {
    std::vector<ECS::Entity> entities_hit;

    auto& positions = reg.getComponent<addon::physic::Position2>();
    auto& hitboxs = reg.getComponent<addon::physic::Hitbox>();
    auto& teams = reg.getComponent<addon::eSpec::Team>();
    auto& track = reg.getComponent<AutoTrack>();
    auto& damage = reg.getComponent<StatPool>();

    auto e_hit = true_hitbox(positions.getComponent(entity),
        hitboxs.getComponent(entity));
    auto& e_team = teams.getComponent(entity);

    for (auto &&[e, pos, hit, tm, _]
        : ECS::IndexedDenseZipper(positions, hitboxs, teams, track)) {
        if (tm.name == e_team.name)
            continue;
        if (square_hitbox(e_hit, true_hitbox(pos, hit))) {
            entities_hit.push_back(e);
        }
    }
    return entities_hit;
}

void dealDamage(Game &game) {
    game.createSystem("deal_damage", [&game](ECS::Registry&){
        auto& champions = game.getComponent<Champion>();
        auto& pools = game.getComponent<StatPool>();
        auto& healths = game.getComponent<Health>();
        auto& vels = game.getComponent<addon::physic::Velocity2>();
        std::vector<ECS::Entity> deads;

        for (auto&& [e, _, hp] : ECS::IndexedDenseZipper(champions, healths)) {
            for (auto& hit : entity_hit_team(game, e)) {
                if (!pools.hasComponent(hit))
                    return;
                auto& damage = pools.getComponent(hit);
                hp.reduceSafely(damage.ad);
                if (hp.amount <= 0) {
                    deads.push_back(e);
                }
                deads.push_back(hit);
            }
            for (auto& dead : deads) {
                game.AddKillEntity(dead);
            }
        }
    });
}
