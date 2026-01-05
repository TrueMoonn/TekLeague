/*
** EPITECH PROJECT, 2025
** GameTwo
** File description:
** entities.hpp
*/

#pragma  once

    #include <unordered_map>
    #include <ECS/Entity.hpp>

    #define MENU_FIELD_SIZE 100
    #define HUD_FIELD_SIZE 100
    #define WEAPON_FIELD_SIZE 1000
    #define MAP_FIELD_SIZE 5000
    #define MOB_FIELD_SIZE 4000
    #define XP_FIELD_SIZE 4000

enum EntityType {
    SYSTEM = 0,
    PLAYER,
    MENU,
    HUD,
    WEAPON,
    MAP,
    MOB,
    XP,
};

enum eField {
    SYSTEM_F = 0,
    PLAYER_F = SYSTEM_F + 1,
    MENU_BEGIN = PLAYER_F + 1,
    MENU_END = MENU_BEGIN + MENU_FIELD_SIZE,
    HUD_BEGIN = MENU_END + 1,
    HUD_END = HUD_BEGIN + HUD_FIELD_SIZE,
    WEAPON_BEGIN = HUD_END + 1,
    WEAPON_END = WEAPON_BEGIN + WEAPON_FIELD_SIZE,
    MAP_BEGIN = WEAPON_END + 1,
    MAP_END = MAP_BEGIN + MAP_FIELD_SIZE,
    MOB_BEGIN = MAP_END + 1,
    MOB_END = MOB_BEGIN + MOB_FIELD_SIZE,
    XP_BEGIN = MOB_END + 1,
    XP_END = XP_BEGIN + XP_FIELD_SIZE,
};

struct EntityFieldLink {
    std::size_t min;
    std::size_t max;
};

static const std::unordered_map<EntityType, EntityFieldLink> ENTITY_FIELDS {
    {SYSTEM, {
        static_cast<std::size_t>(SYSTEM_F),
        static_cast<std::size_t>(SYSTEM_F)
    }},
    {PLAYER, {
        static_cast<std::size_t>(PLAYER_F),
        static_cast<std::size_t>(PLAYER_F)
    }},
    {MENU, {
        static_cast<std::size_t>(MENU_BEGIN),
        static_cast<std::size_t>(MENU_END)
    }},
    {HUD, {
        static_cast<std::size_t>(HUD_BEGIN),
        static_cast<std::size_t>(HUD_END)
    }},
    {WEAPON, {
        static_cast<std::size_t>(WEAPON_BEGIN),
        static_cast<std::size_t>(WEAPON_END)
    }},
    {MAP, {
        static_cast<std::size_t>(MAP_BEGIN),
        static_cast<std::size_t>(MAP_END)
    }},
    {MOB, {
        static_cast<std::size_t>(MOB_BEGIN),
        static_cast<std::size_t>(MOB_END)
    }},
    {XP, {
        static_cast<std::size_t>(XP_BEGIN),
        static_cast<std::size_t>(XP_END)
    }}
};
