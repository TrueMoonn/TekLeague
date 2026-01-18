/*
** EPITECH PROJECT, 2025
** TekLeague
** File description:
** auto_attack.hpp
*/

#pragma once

    #include <string>

    #include <Game.hpp>

struct Attack {
    Attack(const std::string& attack_name, bool aa) :
        name(attack_name), automatic(aa) {}
    std::string name;
    bool automatic;
};

void registerAttack(Game& game);
