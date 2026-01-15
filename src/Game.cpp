/*
** EPITECH PROJECT, 2025
** GameTwo
** File description:
** Game.cpp
*/

#include <ECS/Entity.hpp>

#include "configs/entities.hpp"
#include "configs/components.hpp"
#include "configs/systems.hpp"
#include "entities.hpp"
#include "entities_helper.hpp"
#include "Game.hpp"

Game::Game(const std::string& ppath) : _framelimit(FRAME_LIMIT) {
    loadPlugins(ppath);
    std::srand(std::time(0));

    for (auto& cmpt : LOCAL_COMPONENTS)
        cmpt(*this);
    for (auto& sys : LOCAL_SYSTEMS)
        sys(*this);
    for (auto& conf : CONFIG_PATHS)
        addConfig(conf);
    for (auto& map : MAP_PATHS)
        addMap(map);

    EntityHelper::initNextEntities(_nextEntities);


    sub("closed", [this]() {_running = false;});
    _running = true;
}

ECS::Entity Game::nextEntity(eType type) {
    if (_nextEntities.at(type) >= ENTITY_FIELDS.at(type).max) {
        _nextEntities.at(type) = ENTITY_FIELDS.at(type).min;
    } else {
        _nextEntities.at(type) += 1;
    }
    return _nextEntities.at(type);
}

void Game::run() {
    while (_running) {
        if (_framelimit.checkDelay())
            runSystems();
    }
}
