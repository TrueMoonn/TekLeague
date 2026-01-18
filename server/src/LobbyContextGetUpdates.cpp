#include <vector>
#include <print>
#include <optional>
#include <cstdio>

#include <Network/generated_messages.hpp>
#include <Network/Address.hpp>
#include <ECS/DenseZipper.hpp>
#include <physic/components/position.hpp>
#include <entity_spec/components/health.hpp>
#include <interaction/components/player.hpp>

#include "components/building.hpp"
#include "components/champion.hpp"
#include "components/competences/target.hpp"
#include "components/stats/gold.hpp"
#include "components/stats/health.hpp"
#include "components/stats/mana.hpp"
#include "components/stats/stat_pool.hpp"
#include "components/stats/xp.hpp"
#include "components/competences/spells.hpp"
#include "entity_spec/components/team.hpp"
#include "LobbyContext.hpp"
#include "my.hpp"
#include "physic/components/velocity.hpp"

net::PLAYERS_INIT LobbyContext::getPlayersInit() {
    net::PLAYERS_INIT msg;

    auto& game = getLobby();
    auto& positions = game.getComponent<addon::physic::Position2>();
    auto& teams = game.getComponent<addon::eSpec::Team>();
    auto& champs = game.getComponent<Champion>();

    for (auto&& [entity, pos, team, champ] :
        ECS::IndexedDenseZipper(positions, teams, champs)) {
        net::PlayerInit state;

        std::memset(&state, 0, sizeof(state));
        state.team = 0;
        for (std::size_t i = 0; i < TEAMS.size(); ++i) {
            if (team.name == TEAMS[i])
               state.team = i;
        }
        state.champ = 0;
        for (std::size_t i = 0; i < CHAMPIONS.size(); ++i) {
            if (champ.name == CHAMPIONS[i])
               state.champ = i;
        }
        state.id = 0;
        for (auto& client : _player_entities) {
            if (client.second == entity) {
                state.id = client.first;
                break;
            }
        }
        state.entity = static_cast<uint32_t>(entity),
        state.x = pos.x,
        state.y = pos.y,

        msg.players.push_back(state);
    }

    return msg;
}

net::BUILDINGS_INIT LobbyContext::getBuildingsInit() {
    net::BUILDINGS_INIT msg;

    auto& game = getLobby();
    auto& positions = game.getComponent<addon::physic::Position2>();
    auto& teams = game.getComponent<addon::eSpec::Team>();
    auto& buildings = game.getComponent<Building>();

    for (auto&& [entity, pos, team, building] :
        ECS::IndexedDenseZipper(positions, teams, buildings)) {
        net::BuildingInit state;

        std::memset(&state, 0, sizeof(state));
        state.entity = static_cast<uint32_t>(entity),
        state.x = pos.x,
        state.y = pos.y,
        state.team = 0;
        for (std::size_t i = 0; i < TEAMS.size(); ++i) {
            if (team.name == TEAMS[i])
               state.team = i;
        }
        std::snprintf(state.type, sizeof(state.type), "%s", building.name.c_str());
        msg.buildings.push_back(state);
    }

    return msg;
}

net::PLAYERS_UPDATES LobbyContext::getPlayerUpdates() {
    net::PLAYERS_UPDATES msg;

    auto& game = getLobby();
    auto& positions = game.getComponent<addon::physic::Position2>();
    auto& healths = game.getComponent<Health>();
    auto& champs = game.getComponent<Champion>();
    auto& levels = game.getComponent<Xp>();
    auto& manas = game.getComponent<Mana>();
    auto& targets = game.getComponent<Target>();
    auto& velocities = game.getComponent<addon::physic::Velocity2>();

    for (auto&& [entity, pos, health, champ, level, mana, target, vel] :
        ECS::IndexedDenseZipper(
            positions, healths, champs, levels, manas, targets, velocities)) {
        net::PlayerUpdate state;

        std::memset(&state, 0, sizeof(state));
        state.entity = static_cast<uint32_t>(entity),
        state.x = pos.x,
        state.y = pos.y,
        state.vel_x = vel.x;
        state.vel_y = vel.y;
        state.hp = static_cast<double>(health.amount),
        state.level = level.amount,
        state.mana = mana.amount,
        state.direction_x = target.x,
        state.direction_y = target.y,
        std::memset(state.effects, 0, sizeof(state.effects));

        msg.players.push_back(state);
    }

    return msg;
}

net::ENTITIES_CREATED LobbyContext::getEntitiesCreated() {
    net::ENTITIES_CREATED msg;

    auto& game = getLobby();
    auto& positions = game.getComponent<addon::physic::Position2>();

    for (auto& entity : game.entities_queue) {
        auto& pos = positions.getComponent(static_cast<uint32_t>(entity.first));
        net::EntitiesCreated state;
        std::memset(&state, 0, sizeof(state));
        state.entity = static_cast<uint32_t>(entity.first);
        state.x = pos.x;
        state.y = pos.y;
        std::strncpy(state.type, entity.second.data(), sizeof(state.type) - 1);
        state.type[sizeof(state.type) - 1] = '\0';
        msg.entities.push_back(state);
    }
    game.entities_queue.clear();
    return msg;
}

net::ENTITIES_DESTROYED LobbyContext::getEntitiesDestroyed() {
    net::ENTITIES_DESTROYED msg;

    auto& game = getLobby();

    for (auto& entity : game._EntityToKill) {
        msg.entities.push_back(entity);
    }
    game._EntityToKill.clear();
    return msg;
}

net::BUILDINGS_UPDATES LobbyContext::getBuildingsUpdates() {
    net::BUILDINGS_UPDATES msg;

    auto& game = getLobby();
    auto& healths = game.getComponent<Health>();
    auto& buildings = game.getComponent<Building>();

    for (auto&& [entity, health, _] :
        ECS::IndexedDenseZipper(healths, buildings)) {
        net::BuildingsUpdate state;
        std::memset(&state, 0, sizeof(state));
        state.entity = static_cast<uint32_t>(entity);
        state.hp = static_cast<double>(health.amount);

        msg.buildings.push_back(state);
    }

    return msg;
}

net::CREATURES_UPDATES LobbyContext::getCreaturesUpdates() {
    net::CREATURES_UPDATES msg;

    auto& game = getLobby();
    auto& healths = game.getComponent<Health>();
    auto& positions = game.getComponent<addon::physic::Position2>();

    for (auto&& [entity, position, health] :
        ECS::IndexedDenseZipper(positions, healths)) {
        if (entity < eField::CREATURES_BEGIN)
            continue;
        if (entity > eField::CREATURES_END)
            break;

        net::CreatureUpdate state;
        std::memset(&state, 0, sizeof(state));
        state.entity = static_cast<uint32_t>(entity);
        state.x = position.x;
        state.y = position.y;
        state.hp = health.amount;
        msg.creatures.push_back(state);
    }

    return msg;
}

net::PROJECTILES_UPDATES LobbyContext::getProjectilesUpdates() {
    net::PROJECTILES_UPDATES msg;

    auto& game = getLobby();
    auto& positions = game.getComponent<addon::physic::Position2>();

    for (auto&& [entity, position] :
        ECS::IndexedDenseZipper(positions)) {
        if (entity < eField::PROJECTILES_BEGIN)
            continue;
        if (entity > eField::PROJECTILES_END)
            break;
        net::ProjectileUpdate state;
        std::memset(&state, 0, sizeof(state));
        state.entity = static_cast<uint32_t>(entity);
        state.x = position.x;
        state.y = position.y;

        msg.projectiles.push_back(state);
    }

    return msg;
}

net::COLLECTIBLES_UPDATES LobbyContext::getCollectiblesUpdates() {
    net::COLLECTIBLES_UPDATES msg;

    auto& registry = const_cast<ECS::Registry&>(lobby.getRegistry());
    auto& positions = registry.getComponents<addon::physic::Position2>();

    for (auto&& [entity, position] :
        ECS::IndexedDenseZipper(positions)) {
        if (entity < eField::COLLECTIBLES_BEGIN ||
           entity > eField::COLLECTIBLES_END)
            continue;

        // TODO(x): fill state with real all data
        net::CollectibleUpdate state;
        std::memset(&state, 0, sizeof(state));
        state.id = static_cast<uint32_t>(entity);
        state.x = position.x;
        state.y = position.y;

        msg.collectibles.push_back(state);
    }

    return msg;
}

net::INVENTORIES_UPDATES LobbyContext::getInventoriesUpdates() {
    net::INVENTORIES_UPDATES msg;

    auto& registry = const_cast<ECS::Registry&>(lobby.getRegistry());
    auto& players = registry.getComponents<addon::intact::Player>();

    for (auto&& [entity, player] :
        ECS::IndexedDenseZipper(players)) {
        if (entity < eField::CHAMPION_BEGIN ||
           entity > eField::CHAMPION_END)
            continue;

        // TODO(x): fill state with real all data
        net::InventoryUpdate state;
        std::memset(&state, 0, sizeof(state));
        state.playerid = static_cast<uint32_t>(entity);
        // items array and gold are already zeroed by memset

        msg.inventories.push_back(state);
    }

    return msg;
}

net::STATS_UPDATES LobbyContext::getStatsUpdates() {
    net::STATS_UPDATES msg;

    auto& registry = const_cast<ECS::Registry&>(lobby.getRegistry());
    auto& players = registry.getComponents<addon::intact::Player>();

    for (auto&& [entity, player] :
        ECS::IndexedDenseZipper(players)) {
        if (entity < eField::CHAMPION_BEGIN ||
           entity > eField::CHAMPION_END)
            continue;

        // TODO(x): fill state with real all data
        net::StatsUpdate state;
        std::memset(&state, 0, sizeof(state));
        state.playerid = static_cast<uint32_t>(entity);
        // All other fields are already zeroed by memset

        msg.stats.push_back(state);
    }

    return msg;
}

net::SCORE LobbyContext::getScore() {
    net::SCORE msg;

    msg.team_count = sizeof(lobby.teams) / sizeof(net::TeamScore);
    msg.teams[0] = lobby.teams[0];
    msg.teams[1] = lobby.teams[1];

    return msg;
}

net::GAME_END LobbyContext::getGameEnd() {
    net::GAME_END msg;

    auto pending = lobby.consumePendingWinningTeam();
    msg.winning_team = pending.value_or(0);

    return msg;
}

net::GAME_DURATION LobbyContext::getGameDuration() {
    net::GAME_DURATION msg;

    // TODO(x): use te::Timestamp to calculate elapsed time ?
    auto now = std::chrono::system_clock::now();
    auto elapsed = now - lobby.game_start_time;
    msg.duration = static_cast<uint32_t>(
        std::chrono::duration_cast<std::chrono::seconds>(elapsed).count());

    return msg;
}

net::SCOREBOARD LobbyContext::getScoreboard() {
    net::SCOREBOARD msg;

    for (auto scoreboardentry : lobby.scoreboard) {
        msg.entries.push_back(scoreboardentry);
    }
    return msg;
}

std::optional<net::ENTITIES_CREATED> LobbyContext::tryGetEntitiesCreated() {
    auto& game = getLobby();
    if (!entities_created.checkDelay() || game.entities_queue.empty())
        return std::nullopt;

    return getEntitiesCreated();
}

std::optional<net::ENTITIES_DESTROYED> LobbyContext::tryGetEntitiesDestroyed() {
    auto& game = getLobby();
    if (!entities_destroyed.checkDelay() || game._EntityToKill.empty())
        return std::nullopt;

    return getEntitiesDestroyed();
}

std::optional<net::PLAYERS_UPDATES> LobbyContext::tryGetPlayerUpdates() {
    if (!players_update.checkDelay())
        return std::nullopt;

    return getPlayerUpdates();
}

std::optional<net::BUILDINGS_UPDATES> LobbyContext::tryGetBuildingsUpdates() {
    if (!buildings_update.checkDelay())
        return std::nullopt;

    return getBuildingsUpdates();
}

std::optional<net::CREATURES_UPDATES> LobbyContext::tryGetCreaturesUpdates() {
    if (!creatures_update.checkDelay())
        return std::nullopt;

    return getCreaturesUpdates();
}

std::optional<net::PROJECTILES_UPDATES>
LobbyContext::tryGetProjectilesUpdates(
) {
    if (!projectiles_update.checkDelay())
        return std::nullopt;

    return getProjectilesUpdates();
}

std::optional<net::COLLECTIBLES_UPDATES>
LobbyContext::tryGetCollectiblesUpdates() {
    if (!creatures_update.checkDelay())
        return std::nullopt;

    return getCollectiblesUpdates();
}

std::optional<net::INVENTORIES_UPDATES>
LobbyContext::tryGetInventoriesUpdates() {
    if (!creatures_update.checkDelay())
        return std::nullopt;

    return getInventoriesUpdates();
}

std::optional<net::STATS_UPDATES> LobbyContext::tryGetStatsUpdates() {
    if (!creatures_update.checkDelay())
        return std::nullopt;

    return getStatsUpdates();
}

std::optional<net::SCORE> LobbyContext::tryGetScore() {
    if (!score_update.checkDelay())
        return std::nullopt;

    return getScore();
}

std::optional<net::GAME_END> LobbyContext::tryGetGameEnd() {
    auto pending = lobby.consumePendingWinningTeam();
    if (!pending.has_value())
        return std::nullopt;

    lobby.setGameState(LobbyGameState::END_GAME);

    net::GAME_END msg;
    msg.winning_team = pending.value();

    return msg;
}

std::optional<net::GAME_DURATION> LobbyContext::tryGetGameDuration() {
    if (!game_duration_update.checkDelay())
        return std::nullopt;

    return getGameDuration();
}

std::optional<net::SCOREBOARD> LobbyContext::tryGetScoreboard() {
    if (!scoreboard_update.checkDelay())
        return std::nullopt;

    return getScoreboard();
}

bool LobbyContext::shouldSendPlayersList() {
    if (lobby.getGameState() != LobbyGameState::PRE_GAME)
        return false;

    return players_list_update.checkDelay();
}
