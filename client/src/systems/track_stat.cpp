/*
** EPITECH PROJECT, 2025
** GameTwo
** File description:
** kill_all_entity.hpp
*/

#include <ECS/DenseZipper.hpp>
#include <physic/components/velocity.hpp>
#include <physic/components/position.hpp>
#include <display/components/animation.hpp>
#include <sfml/components/sprite.hpp>

#include "ECS/Registry.hpp"
#include "components/champion.hpp"
#include "components/stats/health.hpp"
#include "components/ui/game/track_stat.hpp"
#include "configs/components.hpp"
#include "client_systems.hpp"

void trackStats(Game& game) {
    game.createSystem("track_stat", [&game](ECS::Registry&){
        auto& trackstat = game.getComponent<TrackStat>();
        auto& health = game.getComponent<Health>();
        auto& mana = game.getComponent<Mana>();
        auto& animation = game.getComponent<addon::display::Animation>();

        for (auto&& [id, anim, track]
            : ECS::IndexedDenseZipper(animation, trackstat)) {
            if (track.id == 0)
                continue;

            float calc = 0;
            if (track.name.compare("health") == 0
            && health.hasComponent(track.id)) {
                auto &hp = health.getComponent(track.id);
                calc = (float(hp.amount) / hp.max_amount) * 100;
                std::cerr << "TS > calc at " << calc << " from " << hp.amount << "/" << hp.max_amount << std::endl;
            } else if (track.name.compare("mana") == 0
            && mana.hasComponent(track.id)) {
                auto &mn = mana.getComponent(track.id);
                calc = (float(mn.amount) / mn.amount) * 100;
            } else {
                game.AddKillEntity(id);
                continue;
            }

            std::size_t cframe = std::abs((calc / 10) - 10);
            std::cerr << "TS > Setting at frame " << cframe << " from calc % " << calc << std::endl;
            if (anim.curAnim != cframe) {
                anim.curAnim = cframe;
            }
        }
    }, false);
}
