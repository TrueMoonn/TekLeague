/*
** EPITECH PROJECT, 2025
** TekLeague
** File description:
** Server.cpp
*/

#include <cstdint>
#include <string>
#include <vector>
#include <random>

#include <ECS/DenseSA.hpp>
#include <ECS/DenseZipper.hpp>

#include <Network/generated_messages.hpp>

#include <physic/components/position.hpp>
#include <entity_spec/components/health.hpp>
#include <entity_spec/components/team.hpp>
#include <interaction/components/player.hpp>

#include <network/GameServer.hpp>

#include "Server.hpp"

Server::Server(uint16_t port, const std::string& protocol) :
    te::network::GameServer(port, protocol) {
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

void Server::createLobby(uint max_clients) {
    std::string code = generateUniqueLobbyCode();
    lobby_codes[code] = next_lobby_id;
    lobbies.try_emplace(next_lobby_id, max_clients, code);
    next_lobby_id++;
}

void Server::destroyLobby(uint lobby_id) {
    if (lobbies.find(lobby_id) != lobbies.end()) {
        std::string code = lobbies.at(lobby_id).getCode();
        lobby_codes.erase(code);
        lobbies.erase(lobby_id);
    }
}

void Server::broadcastToLobby(uint lobby_id, const std::vector<uint8_t>& data) {
    if (lobbies.find(lobby_id) == lobbies.end()) return;

    for (const auto& [client_id, address] : lobbies.at(lobby_id).getClients()) {
        sendTo(address, data);
    }
}

std::string Server::generateUniqueLobbyCode() {
    static const char alphanum[] =
        "0123456789"
        "ABCDEFGHIJKLMNOPQRSTUVWXYZ"
        "abcdefghijklmnopqrstuvwxyz";
    static const int alphanum_size = sizeof(alphanum) - 1;

    static std::random_device rd;
    static std::mt19937 gen(rd());
    static std::uniform_int_distribution<> dis(0, alphanum_size - 1);

    std::string code;
    const int CODE_LENGTH = 6;

    do {
        code.clear();
        for (int i = 0; i < CODE_LENGTH; ++i) {
            code += alphanum[dis(gen)];
        }
    } while (lobby_codes.find(code) != lobby_codes.end());

    return code;
}

void Server::handleConnectionRequest(const std::vector<uint8_t>& data,
    const net::Address& sender) {

}

void Server::sendAutomatic() {
    for (auto& [id, ctx] : lobbies) {
        if (auto msg = ctx.tryGetPlayerUpdates()) {
            broadcastToLobby(id, msg->serialize());
        }
    }
}

void Server::sendPlayersUpdate() {
    for (auto& [id, ctx] : lobbies) {
        if (auto msg = ctx.tryGetPlayerUpdates()) {
            if (!msg->players.empty()) {
                broadcastToLobby(id, msg->serialize());
            }
        }
    }
}

void Server::run() {
    while (isRunning()) {
        // Run game logic for each lobby
        for (auto& [id, ctx] : lobbies) {
            ctx.run();
        }

        update(0);
        sendAutomatic();
    }
}
