/*
** EPITECH PROJECT, 2025
** TekLeague
** File description:
** client_components.hpp
*/

#pragma once

    #include <vector>
    #include <functional>

    #include <GameTool.hpp>

    #include "components/ui/button.hpp"

/**
 * @brief List of components used by the client for UI/menus
 * These are registered automatically in the Client constructor
 */
static const std::vector<std::function<void(te::GameTool&)>> CLIENT_COMPONENTS = {
    &registerButton,
};