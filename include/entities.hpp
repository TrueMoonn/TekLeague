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
    #define MENU_FIELD_SIZE 100
    #define HUD_FIELD_SIZE 100
    #define WEAPON_FIELD_SIZE 1000
    #define MAP_FIELD_SIZE 5000
    #define MOB_FIELD_SIZE 4000
    #define XP_FIELD_SIZE 4000

enum class eType {
    SYSTEM = 0,
    CHAMPION,
    MENU,
    HUD,
    WEAPON,
    MAP,
    MOB,
    XP,
};

enum eField {
    SYSTEM_F = 0,
    CHAMPION_BEGIN = SYSTEM_F + 1,
    CHAMPION_END = CHAMPION_BEGIN + CHAMPION_FIELD_SIZE,
    MENU_BEGIN = CHAMPION_BEGIN + 1,
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
    {eType::WEAPON, {
        static_cast<std::size_t>(WEAPON_BEGIN),
        static_cast<std::size_t>(WEAPON_END)
    }},
    {eType::MAP, {
        static_cast<std::size_t>(MAP_BEGIN),
        static_cast<std::size_t>(MAP_END)
    }},
    {eType::MOB, {
        static_cast<std::size_t>(MOB_BEGIN),
        static_cast<std::size_t>(MOB_END)
    }},
    {eType::XP, {
        static_cast<std::size_t>(XP_BEGIN),
        static_cast<std::size_t>(XP_END)
    }}
};
