/*
** EPITECH PROJECT, 2025
** GameTwo
** File description:
** latencies.hpp
*/

#pragma once

////// Timestamp default latency in seconds //////

static constexpr float PLAYERS_UPDATES_DEFAULT_LATENCY = 1.0f / 120.0f;
static constexpr float BUILDINGS_UPDATES_DEFAULT_LATENCY = 1.0f / 10.0f;
static constexpr float ENTITIES_CREATED_DEFAULT_LATENCY = 1.0f / 50.0f;
static constexpr float ENTITIES_DESTROYED_DEFAULT_LATENCY = 1.0f / 50.0f;
static constexpr float CREATURES_UPDATES_DEFAULT_LATENCY = 1.0f / 60.0f;
static constexpr float PROJECTILES_UPDATES_DEFAULT_LATENCY = 1.0f / 60.0f;
static constexpr float COLLECTIBLES_UPDATES_DEFAULT_LATENCY = 1.0f / 10.0f;
static constexpr float INVENTORIES_UPDATES_DEFAULT_LATENCY = 1.0f / 10.0f;
static constexpr float STATS_UPDATES_DEFAULT_LATENCY = 1.0f / 10.0f;

static constexpr float SCORE_DEFAULT_LATENCY = 1.0f / 1.0f;
static constexpr float GAME_DURATION_DEFAULT_LATENCY = 1.0f / 1.0f;
static constexpr float SCOREBOARD_DEFAULT_LATENCY = 1.0f / 1.0f;

static constexpr float PLAYERS_LIST_DEFAULT_LATENCY = 0.5f;
