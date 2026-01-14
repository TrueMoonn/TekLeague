/*
** EPITECH PROJECT, 2025
** GameTwo
** File description:
** entities.hpp
*/

#pragma  once

    #include <unordered_map>
    #include <ECS/Entity.hpp>

    #define CHAMPION_FIELD_SIZE 6
    #define MENU_FIELD_SIZE 1000
    #define HUD_FIELD_SIZE 100
    #define MAP_FIELD_SIZE 5000
    #define CREATURES_FIELD_SIZE 4000
    #define BUILDINGS_FIELD_SIZE 4000
    #define PROJECTILE_FIELD_SIZE 4000

enum class eType {
    SYSTEM = 0,
    CHAMPION,
    MENU,
    HUD,
    MAP,
    CREATURES,
    BUILDINGS,
    PROJECTILES,
};

enum eField {
    SYSTEM_F = 0,
    CHAMPION_BEGIN = SYSTEM_F + 1,
    CHAMPION_END = CHAMPION_BEGIN + CHAMPION_FIELD_SIZE,
    MENU_BEGIN = CHAMPION_END + 1,
    MENU_END = MENU_BEGIN + MENU_FIELD_SIZE,
    HUD_BEGIN = MENU_END + 1,
    HUD_END = HUD_BEGIN + HUD_FIELD_SIZE,
    MAP_BEGIN = HUD_END + 1,
    MAP_END = MAP_BEGIN + MAP_FIELD_SIZE,
    CREATURES_BEGIN = MAP_END + 1,
    CREATURES_END = CREATURES_BEGIN + CREATURES_FIELD_SIZE,
    BUILDINGS_BEGIN = CREATURES_END + 1,
    BUILDINGS_END = BUILDINGS_BEGIN + BUILDINGS_FIELD_SIZE,
    PROJECTILES_BEGIN = BUILDINGS_END + 1,
    PROJECTILES_END = PROJECTILES_BEGIN + PROJECTILE_FIELD_SIZE,
};

struct EntityFieldLink {
    std::size_t min;
    std::size_t max;
};

static const std::unordered_map<eType, EntityFieldLink> ENTITY_FIELDS {
    {eType::SYSTEM, {
        static_cast<std::size_t>(SYSTEM_F),
        static_cast<std::size_t>(SYSTEM_F)
    }},
    {eType::CHAMPION, {
        static_cast<std::size_t>(CHAMPION_BEGIN),
        static_cast<std::size_t>(CHAMPION_END)
    }},
    {eType::MENU, {
        static_cast<std::size_t>(MENU_BEGIN),
        static_cast<std::size_t>(MENU_END)
    }},
    {eType::HUD, {
        static_cast<std::size_t>(HUD_BEGIN),
        static_cast<std::size_t>(HUD_END)
    }},
    {eType::MAP, {
        static_cast<std::size_t>(MAP_BEGIN),
        static_cast<std::size_t>(MAP_END)
    }},
    {eType::CREATURES, {
        static_cast<std::size_t>(CREATURES_BEGIN),
        static_cast<std::size_t>(CREATURES_END)
    }},
    {eType::BUILDINGS, {
        static_cast<std::size_t>(BUILDINGS_BEGIN),
        static_cast<std::size_t>(BUILDINGS_END)
    }},
    {eType::PROJECTILES, {
        static_cast<std::size_t>(PROJECTILES_BEGIN),
        static_cast<std::size_t>(PROJECTILES_END)
    }}
};
