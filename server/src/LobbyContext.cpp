#include <unordered_map>
#include <string>
#include <print>

#include <Network/generated_messages.hpp>
#include <Network/Address.hpp>
#include <ECS/DenseZipper.hpp>
#include <physic/components/position.hpp>
#include <entity_spec/components/health.hpp>
#include <interaction/components/player.hpp>

#include "LobbyContext.hpp"
#include "ECS/Entity.hpp"
#include "clock.hpp"
#include "components/competences/target.hpp"
#include "components/minions/minions.hpp"
#include "entities.hpp"
#include "systems.hpp"
#include "entity_spec/components/team.hpp"
#include "components/building.hpp"
#include "my.hpp"

LobbyContext::LobbyContext(uint max_players, const std::string& code)
    : lobby(max_players, code, PLUGINS_PATH), max_clients(max_players) {
    for (auto& sys : SERVER_SYSTEMS)
        lobby.createSystem(sys);
}

void LobbyContext::run() {
    SpawnCreeps();
    lobby.run();
}

void LobbyContext::SpawnCreeps() {
    if (!this->spawn_creap.checkDelay(false)) {
        return;
    }
    static te::Timestamp first_creep(0.f);
    static te::Timestamp second_creep((float)0.5);
    static te::Timestamp third_creep(1.f);
    static te::Timestamp fourth_creep((float)1.5);

    for (size_t i = 0; i < CREATURES.size(); ++i) {
        const auto& [mob_name, mob_pos] = CREATURES[i];
        auto& game = getLobby();
        if (first_creep.checkDelay()) {
            ECS::Entity e = game.nextEntity(eType::CREATURES);

            game.createEntity(e, mob_name, {mob_pos.x, mob_pos.y});
            game.entities_queue.emplace_back(e, mob_name);
        }
        if (second_creep.checkDelay()) {
            ECS::Entity e = game.nextEntity(eType::CREATURES);

            game.createEntity(e, mob_name, {mob_pos.x, mob_pos.y});
            game.entities_queue.emplace_back(e, mob_name);
        }
        if (third_creep.checkDelay()) {
            ECS::Entity e = game.nextEntity(eType::CREATURES);

            game.createEntity(e, mob_name, {mob_pos.x, mob_pos.y});
            game.entities_queue.emplace_back(e, mob_name);
        }
        if (fourth_creep.checkDelay()) {
            ECS::Entity e = game.nextEntity(eType::CREATURES);

            game.createEntity(e, mob_name, {mob_pos.x, mob_pos.y});
            game.entities_queue.emplace_back(e, mob_name);
            this->spawn_creap.restart();
        }
    }
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
        game.createEntity(e, "Gules");

        _player_entities[player.id] = e;

        for (auto& plist : game.getPlayers()) {
            teams.getComponent(e).name = TEAMS[plist.team];
            if (plist.team == 1) {
                positions.getComponent(e).x = BLUE_POS_X;
                positions.getComponent(e).y = BLUE_POS_Y;
                targets.getComponent(e).x = BLUE_POS_X;
                targets.getComponent(e).y = BLUE_POS_Y;
            } else {
                positions.getComponent(e).x = RED_POS_X;
                positions.getComponent(e).y = RED_POS_Y;
                targets.getComponent(e).x = RED_POS_X;
                targets.getComponent(e).y = RED_POS_Y;
            }
        }
    }
}

ECS::Entity LobbyContext::getPlayerEntity(uint32_t client_id) const {
    auto it = _player_entities.find(client_id);
    return it->second;
}
