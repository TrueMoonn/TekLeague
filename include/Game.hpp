/*
** EPITECH PROJECT, 2025
** GameTwo
** File description:
** Game.hpp
*/

#pragma once

    #include <unordered_map>

    #include <ECS/Entity.hpp>
    #include <GameTool.hpp>
    #include <clock.hpp>
#include <vector>

    #include "entities.hpp"

    #define FRAME_LIMIT 1.0f / 60   // 60 fps

class Game : public te::GameTool {
 public:
    Game(const std::string& ppath);

    ECS::Entity nextEntity(eType type);


    void AddKillEntity(ECS::Entity e) {
        this->_EntityToKill.push_back(e);
    };
    std::vector<ECS::Entity> getAllEntityToKill() {
        return this->_EntityToKill;
    };

    virtual void run();

 protected:
    bool _running;
    te::Timestamp _framelimit;
 private:
    std::unordered_map<eType, ECS::Entity> _nextEntities;
    std::vector<ECS::Entity> _EntityToKill;
};
