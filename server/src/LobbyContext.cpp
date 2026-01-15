#include <unordered_map>
#include <string>

#include <Network/generated_messages.hpp>
#include <Network/Address.hpp>
#include <ECS/DenseZipper.hpp>
#include <physic/components/position.hpp>
#include <entity_spec/components/health.hpp>
#include <interaction/components/player.hpp>

#include "LobbyContext.hpp"
#include "ECS/Entity.hpp"
#include "entities.hpp"
#include "entity_spec/components/team.hpp"
#include "my.hpp"

LobbyContext::LobbyContext(uint max_players, const std::string& code)
    : lobby(max_players, code, PLUGINS_PATH), max_clients(max_players) {}

void LobbyContext::run() {
    lobby.run();
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

void LobbyContext::createPlayersEntities() {
    auto& game = getLobby();
    auto& players = game.getPlayers();

    auto& teams = game.getComponent<addon::eSpec::Team>();
    auto& positions = game.getComponent<addon::physic::Position2>();

    for (auto& player : players) {
        ECS::Entity e = game.nextEntity(eType::CHAMPION);
        game.createEntity(e, "Gules");

        _player_entities[player.id] = e;

        for (auto& plist : game.getPlayers()) {
            teams.getComponent(e).name = TEAMS[plist.team];
            if (plist.team == 1) {
                positions.getComponent(e).x = BLUE_POS_X;
                positions.getComponent(e).y = BLUE_POS_Y;
            } else {
                positions.getComponent(e).x = RED_POS_X;
                positions.getComponent(e).y = RED_POS_Y;
            }
        }
    }
}

ECS::Entity LobbyContext::getPlayerEntity(uint32_t client_id) const {
    auto it = _player_entities.find(client_id);
    return it->second;
}
