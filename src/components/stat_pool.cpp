/*
** EPITECH PROJECT, 2025
** GameTwo
** File description:
** stat_pool.cpp
*/

#include "components/stat_pool.hpp"

void registerStatPool(Game& game) {
    game.registerComponent("stat_pool",
        [&game](ECS::Entity e, const toml::table& params) {
        int ad = params["ad"].value_or(0);
        int ap = params["ap"].value_or(0);
        int as = params["atk_speed"].value_or(0);
        int ar = params["armor"].value_or(0);
        int mr = params["magic_resist"].value_or(0);
        int mv = params["mov_speed"].value_or(0);
        game.createComponent<StatPool>(e, ad, ap, as, ar, mr, mv);
    });
}
