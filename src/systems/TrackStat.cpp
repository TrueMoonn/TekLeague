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
#include "configs/systems.hpp"

void TrackStats(Game& game) {
    game.createSystem("track_stat", [&game](ECS::Registry&){
        auto& statTracks = game.getComponent<TrackStat>();
        auto& champions = game.getComponent<Champion>();
        auto& Healths = game.getComponent<Health>();
        auto& anims = game.getComponent<addon::display::Animation>();
        auto& Manas = game.getComponent<Mana>();
        // auto& XPs = game.getComponent<XP>();

        for (auto&& [e_champ, champ, hp, mana] : ECS::IndexedDenseZipper(champions, Healths, Manas)) {
            for (auto&& [e_stat, anim, _] : ECS::IndexedDenseZipper(anims, statTracks)) {
                std::size_t cframe = 0;
                int calc = (hp.amount * 100) / hp.max;
                if (calc == 100)
                    cframe = 0;
                else if (calc >= 90) {
                    cframe = 1;
                }
                else if (calc >= 80) {
                    cframe = 2;
                }
                else if (calc >= 70) {
                    cframe = 3;
                }
                else if (calc >= 60) {
                    cframe = 4;
                }
                else if (calc >= 50) {
                    cframe = 5;
                }
                else if (calc >= 40) {
                    cframe = 6;
                }
                else if (calc >= 30) {
                    cframe = 7;
                }
                else if (calc >= 20) {
                    cframe = 8;
                }
                else if (calc >= 10) {
                    cframe = 9;
                }
                else
                    cframe = 10;
                if (anim.curAnim != cframe) {
                    anim.curAnim = cframe;
                }
            }
        }
    });
}
