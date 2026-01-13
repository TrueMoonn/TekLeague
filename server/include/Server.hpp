/*
** EPITECH PROJECT, 2025
** TekLeague
** File description:
** Server.hpp
*/

#pragma once

    #include <string>
    #include <vector>

    #include <network/GameServer.hpp>
    #include <Network/ProtocolManager.hpp>
    #include "Game.hpp"

class Server : public te::network::GameServer, public Game {
 public:
    explicit Server(uint16_t port = 6767, const std::string& protocol = "UDP");

    int runServer();

 private:
    bool setPacketsHandlers();

    void handleConnectionRequest(const std::vector<uint8_t>& data,
        const net::Address& sender);
};
