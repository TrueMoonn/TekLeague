/*
** EPITECH PROJECT, 2025
** GameTwo
** File description:
** minionsRoute.cpp
*/

#include "components/minions/minionsRoute.hpp"
#include "ECS/Entity.hpp"
#include "maths/Vector.hpp"
#include <queue>
#include <toml++/impl/path.hpp>

void registerMinionRoute(Game &game) {
    game.registerComponent<MinionsRoute>("minion_route", [&game](ECS::Entity e, const toml::table &params) {
        std::queue<mat::Vector2i> positions = {};
        const auto &direction1 = params["direction1"].as_array();
        mat::Vector2i pos1 = direction1 && direction1->size() == 2 ? mat::Vector2i{
            direction1->at(0).value_or<int>(0), direction1->at(1).value_or<int>(0)
        } : mat::Vector2i{0, 0};
        positions.push(pos1);
        const auto &direction2 = params["direction2"].as_array();
        mat::Vector2i pos2 = direction2 && direction2->size() == 2 ? mat::Vector2i{
            direction2->at(0).value_or<int>(0), direction2->at(1).value_or<int>(0)
        } : mat::Vector2i{0, 0};
        positions.push(pos2);
        const auto &direction3 = params["direction3"].as_array();
        mat::Vector2i pos3 = direction3 && direction3->size() == 2 ? mat::Vector2i{
            direction3->at(0).value_or<int>(0), direction3->at(1).value_or<int>(0)
        } : mat::Vector2i{0, 0};
        positions.push(pos3);
        const auto &direction4 = params["direction4"].as_array();
        mat::Vector2i pos4 = direction4 && direction4->size() == 2 ? mat::Vector2i{
            direction4->at(0).value_or<int>(0), direction4->at(1).value_or<int>(0)
        } : mat::Vector2i{0, 0};
        positions.push(pos4);
        game.createComponent<MinionsRoute>(e, positions);
    });
}
