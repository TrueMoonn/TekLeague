/*
** EPITECH PROJECT, 2025
** TekLeague
** File description:
** Server.cpp
*/

#include "Server.hpp"
#include "Network/Server.hpp"

Server::Server() :
    Game("server/plugins"),
    _protocol("./config/protocol.json"),
    _server("UDP", 4242, _protocol) {
    _server.start();
}
