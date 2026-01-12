/*
** EPITECH PROJECT, 2025
** TekLeague
** File description:
** Server.hpp
*/

#pragma once

    #include <string>

    #include <network/GameServer.hpp>
    #include <Network/ProtocolManager.hpp>
    #include "Game.hpp"

class Server : public te::network::GameServer, public Game {
 public:
    Server(uint16_t port, const std::string& protocol = "UDP");

 private:
    net::ProtocolManager _protocol;
};
