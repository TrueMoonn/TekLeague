#include <unordered_map>
#include <string>

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
