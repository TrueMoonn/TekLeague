#include <vector>

#include <Network/generated_messages.hpp>
#include <Network/Address.hpp>
#include <ECS/DenseZipper.hpp>
#include <physic/components/position.hpp>
#include <entity_spec/components/health.hpp>
#include <interaction/components/player.hpp>

#include "components/champion.hpp"
#include "components/competences/target.hpp"
#include "components/stats/gold.hpp"
#include "components/stats/health.hpp"
#include "components/stats/mana.hpp"
#include "components/stats/stat_pool.hpp"
#include "components/stats/xp.hpp"
#include "entity_spec/components/team.hpp"
#include "LobbyContext.hpp"
#include "my.hpp"

net::PLAYERS_INIT LobbyContext::getPlayersInit() {
    net::PLAYERS_INIT msg;

    auto& game = getLobby();
    auto& positions = game.getComponent<addon::physic::Position2>();
    auto& teams = game.getComponent<addon::eSpec::Team>();
    auto& champs = game.getComponent<Champion>();

    for (auto&& [entity, pos, team, champ] :
        ECS::IndexedDenseZipper(positions, teams, champs)) {

        net::PlayerInit state;
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
    auto& healths = game.getComponent<Health>();
    auto& teams = game.getComponent<addon::eSpec::Team>();
    // get le component building

    for (auto&& [entity, pos, health, team] :
        ECS::IndexedDenseZipper(positions, healths, teams)) {
        if (entity < eField::BUILDINGS_BEGIN || entity > eField::BUILDINGS_END)
            continue;

        // TODO(x): fill state with real all data
        net::BuildingInit state;
        state.id = static_cast<uint32_t>(entity),
        state.x = pos.x,
        state.y = pos.y,
        state.hp = static_cast<double>(health.amount),
        state.team = 0;
        // state.team = team.name;  // TODO(PIERRE): conversion team name, uint8_t
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

    for (auto&& [entity, pos, health, champ, level, mana, target] :
        ECS::IndexedDenseZipper(positions, healths, champs, levels, manas, targets)) {

        net::PlayerUpdate state;
        state.entity = static_cast<uint32_t>(entity),
        state.x = pos.x,
        state.y = pos.y,
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

net::BUILDINGS_UPDATES LobbyContext::getBuildingsUpdates() {
    net::BUILDINGS_UPDATES msg;

    auto& game = getLobby();
    auto& healths = game.getComponent<Health>();
    // Get component building puis regarder le name pour tower/nexus

    for (auto&& [entity, health] :
        ECS::IndexedDenseZipper(healths)) {
        if (entity < eField::BUILDINGS_BEGIN || entity > eField::BUILDINGS_END)
            continue;

        net::BuildingsUpdate state {
            .id = static_cast<uint32_t>(entity),
            .hp = static_cast<double>(health.amount)
        };

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
        if (entity < eField::CREATURES_BEGIN || entity > eField::CREATURES_END)
            continue;

        // TODO(x): fill state with real all data
        net::CreatureUpdate state {
            .id = static_cast<uint32_t>(entity),
            .x = position.x,
            .y = position.y,
            .direction = 0,
            .hp = static_cast<double>(health.amount),
            .type = 0,
            .effects = static_cast<uint8_t>(0)
        };

        msg.creatures.push_back(state);
    }

    return msg;
}

net::PROJECTILES_UPDATES LobbyContext::getProjectilesUpdates() {
    net::PROJECTILES_UPDATES msg;

    auto& registry = const_cast<ECS::Registry&>(lobby.getRegistry());
    auto& positions = registry.getComponents<addon::physic::Position2>();

    for (auto&& [entity, position] :
        ECS::IndexedDenseZipper(positions)) {
        if (entity < eField::PROJECTILES_BEGIN ||
           entity > eField::PROJECTILES_END)
            continue;

        // TODO(x): fill state with real all data
        net::ProjectileUpdate state {
            .id = static_cast<uint32_t>(entity),
            .x = position.x,
            .y = position.y,
            .type = 0
        };

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
        net::CollectibleUpdate state {
            .id = static_cast<uint32_t>(entity),
            .x = position.x,
            .y = position.y,
            .type = 0
        };

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
        net::InventoryUpdate state {
            .playerid = static_cast<uint32_t>(entity),
            .items = 0,
            .gold = 0
        };

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
        net::StatsUpdate state {
            .playerid = static_cast<uint32_t>(entity),
            .ad = 0,
            .ap = 0,
            .armor = 0,
            .resist = 0,
            .cdr_mov_speed = 0,
            .atk_speed = 0,
            .range = 0,
        };

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
    if (!creatures_update.checkDelay())
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
