/*
** EPITECH PROJECT, 2025
** TekLeague
** File description:
** LobbyDataManager.cpp
*/

#include "LobbyManager.hpp"
#include <print>
#include <cstring>
#include <algorithm>

LobbyDataManager::LobbyDataManager(te::network::GameClient& network_client)
    : _network_client(network_client) {
}

void LobbyDataManager::setServerAddress(const net::Address& address) {
    _server_address = address;
}

////// NETWORK HELPERS //////

void LobbyDataManager::sendLogin(const std::string& username) {
    net::LOGIN msg;
    std::memset(msg.username, 0, 32);
    std::memcpy(msg.username, username.c_str(), std::min(username.size(), size_t(32)));

    _network_client.sendToServer(msg.serialize());
    std::println("[LobbyData] Sent LOGIN for: {}", username);
}

void LobbyDataManager::sendLogout() {
    net::LOGOUT msg;
    _network_client.sendToServer(msg.serialize());
    std::println("[LobbyData] Sent LOGOUT");
}

void LobbyDataManager::sendCreateLobby() {
    net::CREATE_LOBBY msg;
    _network_client.sendToServer(msg.serialize());
    std::println("[LobbyData] Sent CREATE_LOBBY");
}

void LobbyDataManager::sendJoinLobby(const std::string& code) {
    net::JOIN_LOBBY msg;
    std::memset(msg.lobby_code, 0, 6);
    std::memcpy(msg.lobby_code, code.c_str(), std::min(code.size(), size_t(6)));

    _network_client.sendToServer(msg.serialize());
    std::println("[LobbyData] Sent JOIN_LOBBY for code: {}", code);
}

void LobbyDataManager::sendGetPublicLobbies() {
    net::GET_ALL_PUBLIC_LOBBIES msg;
    _network_client.sendToServer(msg.serialize());
    std::println("[LobbyData] Sent GET_ALL_PUBLIC_LOBBIES");
}

void LobbyDataManager::sendLeaveLobby() {
    net::LEAVE_LOBBY msg;
    _network_client.sendToServer(msg.serialize());
    std::println("[LobbyData] Sent LEAVE_LOBBY");
}

void LobbyDataManager::sendStartGame() {
    net::ADMIN_START_GAME msg;
    _network_client.sendToServer(msg.serialize());
    std::println("[LobbyData] Sent ADMIN_START_GAME");
}

void LobbyDataManager::sendToggleVisibility() {
    net::TOGGLE_LOBBY_PRIVATE_PUBLIC msg;
    _network_client.sendToServer(msg.serialize());
    std::println("[LobbyData] Sent TOGGLE_LOBBY_PRIVATE_PUBLIC");
}

void LobbyDataManager::sendPauseGame() {
    net::ADMIN_PAUSE_GAME msg;
    _network_client.sendToServer(msg.serialize());
    std::println("[LobbyData] Sent ADMIN_PAUSE_GAME");
}

void LobbyDataManager::sendWantThisTeam(uint8_t team) {
    net::WANT_THIS_TEAM msg;
    msg.team = team;
    _network_client.sendToServer(msg.serialize());
    std::println("[LobbyData] Sent WANT_THIS_TEAM for team: {}", static_cast<int>(team));
}

////// MESSAGE HANDLERS (update data only) //////

void LobbyDataManager::handleLoggedIn(const net::LOGGED_IN& msg) {
    _client_id = msg.id;
    std::println("[LobbyData] Logged in with ID: {}", _client_id);
}

void LobbyDataManager::handleLoggedOut(const net::LOGGED_OUT& msg) {
    std::println("[LobbyData] Logged out");
    _username.clear();
    _client_id = 0;
}

void LobbyDataManager::handleUsernameAlreadyTaken(const net::USERNAME_ALREADY_TAKEN& msg) {
    std::println("[LobbyData] Username already taken");
    _username.clear();
}

void LobbyDataManager::handleLobbyCreated(const net::LOBBY_CREATED& msg) {
    _lobby_code = std::string(msg.lobby_code, 6);
    _is_admin = true;
    std::println("[LobbyData] Lobby created: {} (admin)", _lobby_code);
}

void LobbyDataManager::handleLobbyJoined(const net::LOBBY_JOINED& msg) {
    std::println("[LobbyData] Successfully joined lobby: {}", _lobby_code);
}

void LobbyDataManager::handleBadLobbyCode(const net::BAD_LOBBY_CODE& msg) {
    std::println("[LobbyData] Invalid lobby code");
    _lobby_code.clear();
}

void LobbyDataManager::handleLobbyFull(const net::LOBBY_FULL& msg) {
    std::println("[LobbyData] Lobby is full");
    _lobby_code.clear();
}

void LobbyDataManager::handlePlayersList(const net::PLAYERS_LIST& msg) {
    _players_in_lobby = msg.players;
    std::println("[LobbyData] Players list updated ({} players)", _players_in_lobby.size());


    _is_admin = false;
    for (const auto& player : _players_in_lobby) {
        if (player.id == _client_id && player.is_admin != 0) {
            _is_admin = true;
            break;
        }
    }

    for (size_t i = 0; i < _players_in_lobby.size(); ++i) {
        const auto& player = _players_in_lobby[i];
        std::string name(player.username, strnlen(player.username, 32));
        std::println("  [{}] ID={} Team={} Username='{}'{}{}", i, player.id,
                static_cast<int>(player.team), name,
                (player.id == _client_id ? " [ME]" : ""),
                (player.is_admin != 0 ? " [ADMIN]" : ""));
    }
}

void LobbyDataManager::handleLobbiesList(const net::LOBBIES_LIST& msg) {
    _cached_lobbies_list = msg.lobby_codes;
    std::println("[LobbyData] Received lobbies list ({} lobbies)", _cached_lobbies_list.size());

    for (const auto& code : _cached_lobbies_list) {
        std::println("  - {}", code);
    }
}

void LobbyDataManager::handleLobbyVisibilityChanged(const net::LOBBY_VISIBILITY_CHANGED& msg) {
    _lobby_is_public = (msg.is_public != 0);
    std::println("[LobbyData] Lobby visibility changed to: {}",
            (_lobby_is_public ? "PUBLIC" : "PRIVATE"));
}

void LobbyDataManager::handleGameStarting(const net::GAME_STARTING& msg) {
    std::println("[LobbyData] GAME STARTING!");
    _in_game = true;
}

void LobbyDataManager::handleLobbyDestroyed(const net::LOBBY_DESTROYED& msg) {
    std::println("[LobbyData] Lobby has been destroyed");
    _lobby_code.clear();
    _is_admin = false;
    _players_in_lobby.clear();
}

void LobbyDataManager::handleNotAdmin(const net::NOT_ADMIN& msg) {
    std::println("[LobbyData] You don't have admin permissions");
}

void LobbyDataManager::handleAdminGamePaused(const net::ADMIN_GAME_PAUSED& msg) {
    std::println("[LobbyData] Game paused/resumed");
}

void LobbyDataManager::handleTeamFull(const net::TEAM_FULL& msg) {
    std::println("[LobbyData] Team is full");
}
