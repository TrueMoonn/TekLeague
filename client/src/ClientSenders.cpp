/*
** EPITECH PROJECT, 2025
** TekLeague
** File description:
** ClientSenders.cpp
*/

#include <print>
#include <cstring>
#include <algorithm>

#include "Client.hpp"

void Client::sendLogin(const std::string& username) {
    net::LOGIN msg;
    std::memset(msg.username, 0, 32);
    std::memcpy(msg.username, username.c_str(), std::min(username.size(), size_t(32)));

    sendToServer(msg.serialize());
    std::println("[Client] Sent LOGIN for: {}", username);
}

void Client::sendLogout() {
    net::LOGOUT msg;
    sendToServer(msg.serialize());
    std::println("[Client] Sent LOGOUT");
}

void Client::sendCreateLobby() {
    net::CREATE_LOBBY msg;
    sendToServer(msg.serialize());
    std::println("[Client] Sent CREATE_LOBBY");
}

void Client::sendJoinLobby(const std::string& code) {
    net::JOIN_LOBBY msg;
    std::memset(msg.lobby_code, 0, 6);
    std::memcpy(msg.lobby_code, code.c_str(), std::min(code.size(), size_t(6)));

    sendToServer(msg.serialize());
    std::println("[Client] Sent JOIN_LOBBY for code: {}", code);
}

void Client::sendGetPublicLobbies() {
    net::GET_ALL_PUBLIC_LOBBIES msg;
    sendToServer(msg.serialize());
    std::println("[Client] Sent GET_ALL_PUBLIC_LOBBIES");
}

void Client::sendLeaveLobby() {
    net::LEAVE_LOBBY msg;
    sendToServer(msg.serialize());
    std::println("[Client] Sent LEAVE_LOBBY");
}

void Client::sendStartGame() {
    net::ADMIN_START_GAME msg;
    sendToServer(msg.serialize());
    std::println("[Client] Sent ADMIN_START_GAME");
}

void Client::sendToggleVisibility() {
    net::TOGGLE_LOBBY_PRIVATE_PUBLIC msg;
    sendToServer(msg.serialize());
    std::println("[Client] Sent TOGGLE_LOBBY_PRIVATE_PUBLIC");
}

void Client::sendPauseGame() {
    net::ADMIN_PAUSE_GAME msg;
    sendToServer(msg.serialize());
    std::println("[Client] Sent ADMIN_PAUSE_GAME");
}

void Client::sendWantThisTeam(uint8_t team) {
    net::WANT_THIS_TEAM msg;
    msg.team = team;
    sendToServer(msg.serialize());
    std::println("[Client] Sent WANT_THIS_TEAM for team: {}", static_cast<int>(team));
}

void Client::sendPing() {
    net::PING msg;
    sendToServer(msg.serialize());
}

void Client::sendPong() {
    net::PONG msg;
    sendToServer(msg.serialize());
}
