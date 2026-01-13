/*
** EPITECH PROJECT, 2025
** GameTwo
** File description:
** stat_pool.cpp
*/

#include "components/stat_pool.hpp"

void registerStatPool(Game& game) {
    game.registerComponent<StatPool>("stat_pool",
        [&game](ECS::Entity e, const toml::table& params) {
            int hp = params["hp"].value_or(0);
            int hp_max = params["hp_max"].value_or(0);
            int hp_regen = params["hp_regen"].value_or(0);
            int ad = params["ad"].value_or(0);
            int ap = params["ap"].value_or(0);
            int as = params["atk_speed"].value_or(0);
            int atkr = params["atk_range"].value_or(0);
            int cdr = params["cdr"].value_or(0);
            int ar = params["armor"].value_or(0);
            int mr = params["magic_resist"].value_or(0);
            int mv = params["mov_speed"].value_or(0);

        game.createComponent<StatPool>(e, hp, hp_max, hp_regen, ad, ap, as, atkr, cdr, ar, mr, mv);
    });
}
