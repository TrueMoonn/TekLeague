#include <Network/generated_messages.hpp>
#include <Network/Address.hpp>
#include <ECS/DenseZipper.hpp>
#include <physic/components/position.hpp>
#include <entity_spec/components/health.hpp>
#include <interaction/components/player.hpp>

#include "lobby/LobbyContext.hpp"

LobbyContext::LobbyContext(uint max_players, const std::string& code)
    : lobby(max_players, code), max_clients(max_players) {}

void LobbyContext::run() {
    lobby.run();
}

const std::string& LobbyContext::getCode() {
    return lobby.getCode();
}

Lobby& LobbyContext::getLobby() {
    return lobby;
}

net::PLAYERS_UPDATES LobbyContext::getPlayerUpdates() {
    net::PLAYERS_UPDATES msg;

    auto& registry = const_cast<ECS::Registry&>(lobby.getRegistry());
    auto& positions = registry.getComponents<addon::physic::Position2>();
    auto& healths = registry.getComponents<addon::eSpec::Health>();
    auto& players = registry.getComponents<addon::intact::Player>();

    for (auto&& [player, pos, health] :
        ECS::DenseZipper(players, positions, healths)) {
        net::PlayerUpdate state;
        state.x = pos.x;
        state.y = pos.y;
        state.hp = static_cast<double>(health.amount);
        state.id = 0;  // TODO(xxx): à voir comment on gère leur id
        state.level = 0.0;  // TODO(xxx): implement later
        state.mana = 0.0;    // TODO(xxx): implement later
        state.direction = 0;  // TODO(xxx): implement later
        std::memset(state.effects, 0, sizeof(state.effects));  // TODO(xxx): implement later

        msg.players.push_back(state);
    }

    return msg;
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

const std::unordered_map<uint32_t, net::Address>& LobbyContext::getClients() const {
    return connected_players;
}

bool LobbyContext::isFull() const {
    return connected_players.size() >= max_clients;
}

std::optional<net::PLAYERS_UPDATES> LobbyContext::tryGetPlayerUpdates() {
    if (!players_update.checkDelay()) {
        return std::nullopt;
    }

    return getPlayerUpdates();
}
