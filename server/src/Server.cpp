/*
** EPITECH PROJECT, 2025
** TekLeague
** File description:
** Server.cpp
*/

#include <string>
#include <vector>

#include <network/GameServer.hpp>
#include "Server.hpp"

Server::Server(uint16_t port, const std::string& protocol) :
    te::network::GameServer(port, protocol),
    Game("server/plugins") {
    start();
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

int Server::runServer() {
    while (isRunning()) {
        if (_framelimit.checkDelay())
            runSystems();
    }
    return 0;
}
