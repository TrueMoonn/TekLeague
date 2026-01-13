/*
** EPITECH PROJECT, 2025
** TekLeague
** File description:
** Server.cpp
*/

#include <cstdint>
#include <string>
#include <vector>

#include <ECS/DenseSA.hpp>
#include <ECS/DenseZipper.hpp>

#include <Network/generated_messages.hpp>

#include <physic/components/position.hpp>
#include <entity_spec/components/health.hpp>
#include <entity_spec/components/team.hpp>
#include <interaction/components/player.hpp>

#include <network/GameServer.hpp>

#include "Server.hpp"
#include "ECS/Registry.hpp"

Server::Server(uint16_t port, const std::string& protocol) :
    te::network::GameServer(port, protocol)
    , Game("server/plugins")
    , next_player_id(2) {
    start();
    startNetworkThread();
    setPacketsHandlers();
}

bool Server::setPacketsHandlers() {
    registerPacketHandler(1,
        [this](const std::vector<uint8_t>& data, const net::Address& sender) {
            handleConnectionRequest(data, sender);
        });
    return true;
}

void Server::handleConnectionRequest(const std::vector<uint8_t>& data,
    const net::Address& sender) {

}

void Server::sendAutomatic() {
    if (players_update_timestamp.checkDelay()) {
        sendPlayersUpdate();
    }
}

void Server::sendPlayersUpdate() {
    ::net::PLAYERS_UPDATES msg;
    ECS::Registry registry = getRegistry();

    auto& positions = registry.getComponents<addon::physic::Position2>();
    auto& healths = registry.getComponents<addon::eSpec::Health>();
    auto& players = registry.getComponents<addon::intact::Player>();

    for (auto&& [player, pos, health] :
        ECS::DenseZipper(players, positions, healths)) {
        ::net::PlayerUpdate state;
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

    if (!msg.players.empty()) {
        broadcastToAll(msg.serialize());
    }
}

void Server::run() {
    while (isRunning()) {
        if (_framelimit.checkDelay())
            runSystems();
        update(0);
        sendAutomatic();
    }
}
