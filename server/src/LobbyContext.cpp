#include <unordered_map>
#include <string>
#include <print>
#include <vector>

#include <Network/generated_messages.hpp>
#include <Network/Address.hpp>
#include <ECS/DenseZipper.hpp>
#include <physic/components/position.hpp>
#include <entity_spec/components/health.hpp>
#include <interaction/components/player.hpp>

#include "LobbyContext.hpp"
#include "components/competences/target.hpp"
#include "entities.hpp"
#include "systems.hpp"
#include "entity_spec/components/team.hpp"
#include "components/building.hpp"
#include "my.hpp"

LobbyContext::LobbyContext(uint32_t max_players, const std::string& code)
    : lobby(max_players, code, PLUGINS_PATH), max_clients(max_players) {
    for (auto& sys : SERVER_SYSTEMS)
        lobby.createSystem(sys);
}

void LobbyContext::run() {
    try {
        lobby.run();
        checkNash();
    } catch (const std::exception& e) {
        std::println(stderr, "[LobbyContext::run] ERROR: {}", e.what());
    } catch (...) {
        std::println(stderr, "[LobbyContext::run] ERROR: unknown exception");
    }
}

void LobbyContext::checkNash() {
    if (getGameState() == LobbyGameState::PRE_GAME) {
        spawn_nash_delay.restart();
        stay_nash_delay.restart();
        return;
    }
    if (_nashAlive) {
        if (stay_nash_delay.checkDelay()) {
            auto& game = getLobby();
            game.AddKillEntity(_nashE);

            _nashAlive = false;
            _nashE = 0;
            spawn_nash_delay.restart();
        }
    } else {
        if (spawn_nash_delay.checkDelay()) {
            spawnNash();
            _nashAlive = true;
            stay_nash_delay.restart();
        }
    }
}

void LobbyContext::spawnNash() {
    auto& game = getLobby();

    ECS::Entity e = game.nextEntity(eType::CREATURES);

    _nashE = e;
    game.createEntity(e, "nash", {8192 / 2, 400});
    game.entities_queue.emplace_back(e, "nash");
}

const std::string& LobbyContext::getCode() {
    return lobby.getCode();
}

Game& LobbyContext::getLobby() {
    return lobby;
}

bool LobbyContext::addClient(const net::Address& address, uint32_t client_id) {
    if (connected_players.size() >= max_clients)
        return false;
    connected_players[client_id] = address;
    return true;
}

void LobbyContext::removeClient(uint32_t client_id) {
    connected_players.erase(client_id);
}

const std::unordered_map<uint32_t, net::Address>& LobbyContext::getClients(
) const {
    return connected_players;
}

bool LobbyContext::isFull() const {
    return connected_players.size() >= max_clients;
}

void LobbyContext::createOtherEntities() {
    auto& game = getLobby();

    for (size_t i = 0; i < BUILDINGS.size(); ++i) {
        const auto& [tower_name, tower_pos] = BUILDINGS[i];
        ECS::Entity e = game.nextEntity(eType::BUILDINGS);
        game.createEntity(e, tower_name, {tower_pos.x, tower_pos.y});
    }
}

void LobbyContext::createPlayersEntities() {
    auto& game = getLobby();
    auto& players = game.getPlayers();

    auto& teams = game.getComponent<addon::eSpec::Team>();
    auto& positions = game.getComponent<addon::physic::Position2>();
    auto& targets = game.getComponent<Target>();

    for (auto& player : players) {
        ECS::Entity e = game.nextEntity(eType::CHAMPION);
        game.createEntity(e, CHAMPIONS[player.champion]);

        _player_entities[player.id] = e;

        std::size_t team_idx =
            (player.team < TEAMS.size()) ? player.team : 2;
        teams.getComponent(e).name = TEAMS[team_idx];

        if (team_idx == 1) {  // blue
            positions.getComponent(e).x = BLUE_POS_X;
            positions.getComponent(e).y = BLUE_POS_Y;
            targets.getComponent(e).x = BLUE_POS_X;
            targets.getComponent(e).y = BLUE_POS_Y;
        } else {  // red
            positions.getComponent(e).x = RED_POS_X;
            positions.getComponent(e).y = RED_POS_Y;
            targets.getComponent(e).x = RED_POS_X;
            targets.getComponent(e).y = RED_POS_Y;
        }
    }
}

ECS::Entity LobbyContext::getPlayerEntity(uint32_t client_id) const {
    auto it = _player_entities.find(client_id);
    if (it == _player_entities.end())
        return 0;
    return it->second;
}

std::vector<uint8_t> LobbyContext::forceGetData(uint8_t code) {
    switch (code) {
        case static_cast<uint8_t>(net::PLAYERS_UPDATES::ID):
            return getPlayerUpdates().serialize();
        case static_cast<uint8_t>(net::BUILDINGS_UPDATES::ID):
            return getBuildingsUpdates().serialize();
        case static_cast<uint8_t>(net::CREATURES_UPDATES::ID):
            return getCreaturesUpdates().serialize();
        case static_cast<uint8_t>(net::PROJECTILES_UPDATES::ID):
            return getProjectilesUpdates().serialize();
        case static_cast<uint8_t>(net::COLLECTIBLES_UPDATES::ID):
            return getCollectiblesUpdates().serialize();
        case static_cast<uint8_t>(net::INVENTORIES_UPDATES::ID):
            return getInventoriesUpdates().serialize();
        case static_cast<uint8_t>(net::STATS_UPDATES::ID):
            return getStatsUpdates().serialize();
        case static_cast<uint8_t>(net::SCORE::ID):
            return getScore().serialize();
        case static_cast<uint8_t>(net::GAME_DURATION::ID):
            return getGameDuration().serialize();
        case static_cast<uint8_t>(net::SCOREBOARD::ID):
            return getScoreboard().serialize();
        default:
            return {};
    }
}
