/*
** EPITECH PROJECT, 2025
** TekLeague
** File description:
** Server.hpp
*/

#pragma once

    #include <Network/Server.hpp>
    #include <Network/ProtocolManager.hpp>
    #include "Game.hpp"

class Server : public Game {
 public:
    Server();

 private:
    net::ProtocolManager _protocol;
    net::Server _server;
};
