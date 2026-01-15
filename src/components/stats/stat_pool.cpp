/*
** EPITECH PROJECT, 2025
** GameTwo
** File description:
** stat_pool.cpp
*/

#include "components/stats/stat_pool.hpp"

void registerStatPool(Game& game) {
    game.registerComponent<StatPool>("stat_pool",
        [&game](ECS::Entity e, const toml::table& params) {
            int ad = params["ad"].value_or(0);
            int ap = params["ap"].value_or(0);
<<<<<<< HEAD
            float as = params["atk_speed"].value_or(0.f);
=======
            float as = params["atk_speed"].value_or(0);
>>>>>>> debc9ce4e13fbec5cb57e83446577183e8542355
            int atkr = params["atk_range"].value_or(0);
            int cdr = params["cdr"].value_or(0);
            int ar = params["armor"].value_or(0);
            int mr = params["magic_resist"].value_or(0);
            int mv = params["mov_speed"].value_or(0);

        game.createComponent<StatPool>(e, ad, ap, as, atkr, cdr, ar, mr, mv);
    });
}
