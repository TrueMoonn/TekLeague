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
#include "Game.hpp"

Game::Game() : _framelimit(FRAME_LIMIT) {
    loadPlugins();
    std::srand(std::time(0));

    for (auto& cmpt : LOCAL_COMPONENTS)
        cmpt(*this);
    for (auto& sys : LOCAL_SYSTEMS)
        sys(*this);
    for (auto& conf : CONFIG_PATHS)
        addConfig(conf);
    for (auto& map : MAP_PATHS)
        addMap(map);

    _nextEntities[eType::SYSTEM] = eField::SYSTEM_F;
    _nextEntities[eType::CHAMPION] = eField::CHAMPION_BEGIN;
    _nextEntities[eType::MENU] = eField::MENU_BEGIN;
    _nextEntities[eType::HUD] = eField::HUD_BEGIN;
    _nextEntities[eType::WEAPON] = eField::WEAPON_BEGIN;
    _nextEntities[eType::MAP] = eField::MAP_BEGIN;
    _nextEntities[eType::MOB] = eField::MOB_BEGIN;
    _nextEntities[eType::XP] = eField::XP_BEGIN;

    sub("closed", [this]() {_running = false;});
    _running = true;
    createEntity(nextEntity(eType::SYSTEM), "ig_window");
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
