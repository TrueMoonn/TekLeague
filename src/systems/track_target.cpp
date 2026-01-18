/*
** EPITECH PROJECT, 2025
** GameTwo
** File description:
** track_target.hpp
*/

#include <string>

#include <ECS/DenseZipper.hpp>
#include <physic/components/velocity.hpp>
#include <physic/components/position.hpp>
#include <display/components/animation.hpp>
#include <sfml/components/sprite.hpp>

#include "components/competences/auto_attack.hpp"
#include "components/competences/spells.hpp"
#include "components/competences/target.hpp"
#include "components/stats/stat_pool.hpp"
#include "configs/systems.hpp"
#include "entity_spec/components/team.hpp"
#include "maths/Vector.hpp"

void trackTarget(Game& game) {
    game.createSystem("entity_direction", [&game](ECS::Registry&) {
        auto& targets = game.getComponent<Target>();
        auto& stats = game.getComponent<StatPool>();
        auto& spells = game.getComponent<Spell>();
        auto& attacks = game.getComponent<Attack>();
        auto& posis = game.getComponent<addon::physic::Position2>();
        auto& vels = game.getComponent<addon::physic::Velocity2>();

        for (auto&& [e, tag, pos, vel, stat] :
            ECS::IndexedDenseZipper(targets, posis, vels, stats)) {
            mat::Vector2f destination(tag);
            float dist = 0.f;
            if (tag.to_attack != 0 && attacks.hasComponent(e)) {
                if (posis.hasComponent(tag.to_attack)) {
                    destination = posis.getComponent(tag.to_attack);
                    dist = std::sqrt(std::pow(pos.x - destination.x, 2) +
                        std::pow(pos.y - destination.y, 2));
                } else {
                    tag.to_attack = 0;
                    continue;
                }
                if (dist <= stat.atk_range) {
                    if (stat.atk_speed.checkDelay()) {
                        std::size_t target = tag.to_attack;
                        auto& teams = game.getComponent<addon::eSpec::Team>();
                        std::string aname = attacks.getComponent(e).name;
                        vel.x = 0.f;
                        vel.y = 0.f;
                        ECS::Entity projectile =
                            game.nextEntity(eType::PROJECTILES);
                        game.createEntity(projectile, aname, pos);
                        game.getComponent<Target>().
                            getComponent(projectile).to_attack = target;
                        spells.getComponent(projectile).from = e;
                        teams.getComponent(projectile).name =
                            teams.getComponent(e).name;
                        game.entities_queue.emplace_back(projectile, aname);
                        continue;
                    } else {
                        vel.x = 0.f;
                        vel.y = 0.f;
                        continue;
                    }
                }
            } else  {
                dist = std::sqrt(std::pow(pos.x - destination.x, 2) +
                    std::pow(pos.y - destination.y, 2));
                if (dist < 10) {
                    vel.x = 0.f;
                    vel.y = 0.f;
                    continue;
                }
            }
            mat::Vector2f dir(destination - pos);
            float length = std::sqrt(dir.x * dir.x + dir.y * dir.y);
            dir /= length;
            vel.x = dir.x * stat.mov_speed;
            vel.y = dir.y * stat.mov_speed;
        }

        for (auto&& [tag, pos, vel, spell] :
            ECS::DenseZipper(targets, posis, vels, spells)) {
            mat::Vector2f destination(tag);
            if (tag.to_attack != 0 && posis.hasComponent(tag.to_attack))
                destination = posis.getComponent(tag.to_attack);
            float dist = std::sqrt(std::pow(pos.x - destination.x, 2) +
                std::pow(pos.y - destination.y, 2));
            if (dist < 10) {
                vel.x = 0;
                vel.y = 0;
                continue;
            }
            mat::Vector2f dir(destination - pos);
            float length = std::sqrt(dir.x * dir.x + dir.y * dir.y);
            dir /= length;
            vel.x = dir.x * spell.spellSpeed;
            vel.y = dir.y * spell.spellSpeed;
        }
    });
}
