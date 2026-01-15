/*
** EPITECH PROJECT, 2025
** TekLeague
** File description:
** entities_helper.hpp
*/

#pragma once

    #include <unordered_map>
    #include <ECS/Entity.hpp>
    #include "entities.hpp"

namespace EntityHelper {

/**
 * @brief Initialize the _nextEntities map with default starting values
 * This function is used by both Game and Client to initialize entity ID ranges
 * 
 * @param nextEntities The map to initialize (passed by reference)
 */
inline void initNextEntities(std::unordered_map<eType, ECS::Entity>& nextEntities) {
    nextEntities[eType::SYSTEM] = eField::SYSTEM_F;
    nextEntities[eType::CHAMPION] = eField::CHAMPION_BEGIN;
    nextEntities[eType::MENU] = eField::MENU_BEGIN;
    nextEntities[eType::HUD] = eField::HUD_BEGIN;
    nextEntities[eType::MAP] = eField::MAP_BEGIN;
    nextEntities[eType::CREATURES] = eField::CREATURES_BEGIN;
    nextEntities[eType::BUILDINGS] = eField::BUILDINGS_BEGIN;
    nextEntities[eType::PROJECTILES] = eField::PROJECTILES_BEGIN;
}

} // namespace EntityHelper