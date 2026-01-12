/*
** EPITECH PROJECT, 2025
** TekLeague
** File description:
** Server.cpp
*/

#include <string>

#include <network/GameServer.hpp>
#include "Server.hpp"

Server::Server(uint16_t port, const std::string& protocol) :
    te::network::GameServer(port, protocol),
    Game("server/plugins"),
    _protocol("./config/protocol.json") {
    start();
}
