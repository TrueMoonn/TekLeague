/*
** EPITECH PROJECT, 2025
** TekLeague
** File description:
** ClientSenders.cpp
*/

#include <print>
#include <cstring>
#include <algorithm>
#include <string>

#include "Client.hpp"

void Client::sendLogin(const std::string& username) {
    net::LOGIN msg;
    std::memset(msg.username, 0, 32);
    std::memcpy(msg.username, username.c_str(),
        std::min(username.size(), size_t(32)));

    sendToServer(msg.serialize());
}

void Client::sendPacketLoss(uint8_t code) {
    net::PACKET_LOSS msg;
    msg.code = code;

    sendToServer(msg.serialize());
}

void Client::sendLogout() {
    net::LOGOUT msg;
    sendToServer(msg.serialize());
}

void Client::sendCreateLobby() {
    net::CREATE_LOBBY msg;
    sendToServer(msg.serialize());
}

void Client::sendJoinLobby(const std::string& code) {
    net::JOIN_LOBBY msg;
    std::memset(msg.lobby_code, 0, 6);
    std::memcpy(msg.lobby_code, code.c_str(), std::min(code.size(), size_t(6)));

    sendToServer(msg.serialize());
}

void Client::sendGetPublicLobbies() {
    net::GET_ALL_PUBLIC_LOBBIES msg;
    sendToServer(msg.serialize());
}

void Client::sendLeaveLobby() {
    net::LEAVE_LOBBY msg;
    sendToServer(msg.serialize());
}

void Client::sendStartGame() {
    net::ADMIN_START_GAME msg;
    sendToServer(msg.serialize());
}

void Client::sendToggleVisibility() {
    net::TOGGLE_LOBBY_PRIVATE_PUBLIC msg;
    sendToServer(msg.serialize());
}

void Client::sendPauseGame() {
    net::ADMIN_PAUSE_GAME msg;
    sendToServer(msg.serialize());
}

void Client::sendWantThisTeam(uint8_t team) {
    net::WANT_THIS_TEAM msg;
    msg.team = team;
    sendToServer(msg.serialize());
}

void Client::sendPing() {
    net::PING msg;
    sendToServer(msg.serialize());
}

void Client::sendPong() {
    net::PONG msg;
    sendToServer(msg.serialize());
}
