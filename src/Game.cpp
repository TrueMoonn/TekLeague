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

    _nextEntities[eType::SYSTEM] = eField::SYSTEM_F;
    _nextEntities[eType::CHAMPION] = eField::CHAMPION_BEGIN;
    _nextEntities[eType::MENU] = eField::MENU_BEGIN;
    _nextEntities[eType::HUD] = eField::HUD_BEGIN;
    _nextEntities[eType::MAP] = eField::MAP_BEGIN;
    _nextEntities[eType::CREATURES] = eField::CREATURES_BEGIN;
    _nextEntities[eType::BUILDINGS] = eField::BUILDINGS_BEGIN;
    _nextEntities[eType::PROJECTILES] = eField::PROJECTILES_BEGIN;


    sub("closed", [this]() {_running = false;});
    _running = true;
}

ECS::Entity Game::nextEntity(eType type) {
    // Initialiser la clé si elle n'existe pas
    if (_nextEntities.find(type) == _nextEntities.end()) {
        // Initialiser selon le type
        switch (type) {
            case eType::SYSTEM:
                _nextEntities[type] = eField::SYSTEM_F;
                break;
            case eType::CHAMPION:
                _nextEntities[type] = eField::CHAMPION_BEGIN;
                break;
            case eType::MENU:
                _nextEntities[type] = eField::MENU_BEGIN;
                break;
            case eType::HUD:
                _nextEntities[type] = eField::HUD_BEGIN;
                break;
            case eType::MAP:
                _nextEntities[type] = eField::MAP_BEGIN;
                break;
            case eType::MOB:
                _nextEntities[type] = eField::MOB_BEGIN;
                break;
            case eType::BUILDINGS:
                _nextEntities[type] = eField::BUILDINGS_BEGIN;
                break;
            case eType::PROJECTILES:
                _nextEntities[type] = eField::PROJECTILES_BEGIN;
                break;
        }
    }

    if (_nextEntities[type] >= ENTITY_FIELDS.at(type).max) {
        _nextEntities[type] = ENTITY_FIELDS.at(type).min;
    } else {
        _nextEntities[type] += 1;
    }
    return _nextEntities[type];
}

void Game::run() {
    while (_running) {
        if (_framelimit.checkDelay())
            runSystems();
    }
}
