/*
** EPITECH PROJECT, 2025
** TekLeague
** File description:
** LobbyContext.cpp
*/

#include <print>
#include <cstring>
#include <algorithm>

#include <network/GameClient.hpp>

#include "LobbyContext.hpp"

LobbyContext::LobbyContext(te::network::GameClient& network_client)
    : _network_client(network_client)
    , lobby(MAX_PLAYERS, DEFAULT_CODE, PLUGINS_PATH) {
}

void LobbyContext::setServerAddress(const net::Address& address) {
    _server_address = address;
}

////// NETWORK HELPERS //////

void LobbyContext::sendLogin(const std::string& username) {
    net::LOGIN msg;
    std::memset(msg.username, 0, 32);
    std::memcpy(msg.username, username.c_str(), std::min(username.size(), size_t(32)));

    _network_client.sendToServer(msg.serialize());
    std::println("[LobbyData] Sent LOGIN for: {}", username);
}

void LobbyContext::sendLogout() {
    net::LOGOUT msg;
    _network_client.sendToServer(msg.serialize());
    std::println("[LobbyData] Sent LOGOUT");
}

void LobbyContext::sendCreateLobby() {
    net::CREATE_LOBBY msg;
    _network_client.sendToServer(msg.serialize());
    std::println("[LobbyData] Sent CREATE_LOBBY");
}

void LobbyContext::sendJoinLobby(const std::string& code) {
    net::JOIN_LOBBY msg;
    std::memset(msg.lobby_code, 0, 6);
    std::memcpy(msg.lobby_code, code.c_str(), std::min(code.size(), size_t(6)));

    _network_client.sendToServer(msg.serialize());
    std::println("[LobbyData] Sent JOIN_LOBBY for code: {}", code);
}

void LobbyContext::sendGetPublicLobbies() {
    net::GET_ALL_PUBLIC_LOBBIES msg;
    _network_client.sendToServer(msg.serialize());
    std::println("[LobbyData] Sent GET_ALL_PUBLIC_LOBBIES");
}

void LobbyContext::sendLeaveLobby() {
    net::LEAVE_LOBBY msg;
    _network_client.sendToServer(msg.serialize());
    std::println("[LobbyData] Sent LEAVE_LOBBY");
}

void LobbyContext::sendStartGame() {
    net::ADMIN_START_GAME msg;
    _network_client.sendToServer(msg.serialize());
    std::println("[LobbyData] Sent ADMIN_START_GAME");
}

void LobbyContext::sendToggleVisibility() {
    net::TOGGLE_LOBBY_PRIVATE_PUBLIC msg;
    _network_client.sendToServer(msg.serialize());
    std::println("[LobbyData] Sent TOGGLE_LOBBY_PRIVATE_PUBLIC");
}

void LobbyContext::sendPauseGame() {
    net::ADMIN_PAUSE_GAME msg;
    _network_client.sendToServer(msg.serialize());
    std::println("[LobbyData] Sent ADMIN_PAUSE_GAME");
}

void LobbyContext::sendWantThisTeam(uint8_t team) {
    net::WANT_THIS_TEAM msg;
    msg.team = team;
    _network_client.sendToServer(msg.serialize());
    std::println("[LobbyData] Sent WANT_THIS_TEAM for team: {}", static_cast<int>(team));
}

////// MESSAGE HANDLERS (update data only) //////

void LobbyContext::handleLoggedIn(const net::LOGGED_IN& msg) {
    _client_id = msg.id;
    std::println("[LobbyData] Logged in with ID: {}", _client_id);
}

void LobbyContext::handleLoggedOut(const net::LOGGED_OUT& msg) {
    std::println("[LobbyData] Logged out");
    _username.clear();
    _client_id = 0;
}

void LobbyContext::handleUsernameAlreadyTaken(const net::USERNAME_ALREADY_TAKEN& msg) {
    std::println("[LobbyData] Username already taken");
    _username.clear();
}

void LobbyContext::handleLobbyCreated(const net::LOBBY_CREATED& msg) {
    lobby.setCode(std::string(msg.lobby_code, 6));
    _is_admin = true;
    std::println("[LobbyData] Lobby created: {} (admin)", getLobbyCode());
}

void LobbyContext::handleLobbyJoined(const net::LOBBY_JOINED& msg) {
    std::println("[LobbyData] Successfully joined lobby: {}", getLobbyCode());
}

void LobbyContext::handleBadLobbyCode(const net::BAD_LOBBY_CODE& msg) {
    std::println("[LobbyData] Invalid lobby code");
    lobby.setCode("");
}

void LobbyContext::handleLobbyFull(const net::LOBBY_FULL& msg) {
    std::println("[LobbyData] Lobby is full");
    lobby.setCode("");
}

void LobbyContext::handlePlayersList(const net::PLAYERS_LIST& msg) {
    lobby.setPlayers(msg.players);
    std::println("[LobbyData] Players list updated ({} players)", lobby.getPlayers().size());

    _is_admin = false;
    for (const auto& player : lobby.getPlayers()) {
        if (player.id == _client_id && player.is_admin != 0) {
            _is_admin = true;
            break;
        }
    }

    for (size_t i = 0; i < lobby.getPlayers().size(); ++i) {
        const auto& player = lobby.getPlayers()[i];
        std::string name(player.username, strnlen(player.username, 32));
        std::println("  [{}] ID={} Team={} Username='{}'{}{}", i, player.id,
                static_cast<int>(player.team), name,
                (player.id == _client_id ? " [ME]" : ""),
                (player.is_admin != 0 ? " [ADMIN]" : ""));
    }
}

void LobbyContext::handleLobbiesList(const net::LOBBIES_LIST& msg) {
    _cached_lobbies_list = msg.lobby_codes;
    std::println("[LobbyData] Received lobbies list ({} lobbies)", _cached_lobbies_list.size());

    for (const auto& code : _cached_lobbies_list) {
        std::println("  - {}", code);
    }
}

void LobbyContext::handleLobbyVisibilityChanged(const net::LOBBY_VISIBILITY_CHANGED& msg) {
    lobby.setPublic(msg.is_public != 0);
    std::println("[LobbyData] Lobby visibility changed to: {}",
            (lobby.isPublic() ? "PUBLIC" : "PRIVATE"));
}

void LobbyContext::handleGameStarting(const net::GAME_STARTING& msg) {
    std::println("[LobbyData] GAME STARTING!");
    lobby.setGameState(LobbyGameState::IN_GAME);
}

void LobbyContext::handleLobbyDestroyed(const net::LOBBY_DESTROYED& msg) {
    std::println("[LobbyData] Lobby has been destroyed");
    lobby.setCode("");
    _is_admin = false;
    lobby.clearPlayers();
    lobby.setGameState(LobbyGameState::PRE_GAME);
}

void LobbyContext::handleNotAdmin(const net::NOT_ADMIN& msg) {
    std::println("[LobbyData] You don't have admin permissions");
}

void LobbyContext::handleAdminGamePaused(const net::ADMIN_GAME_PAUSED& msg) {
    std::println("[LobbyData] Game paused/resumed");
}

void LobbyContext::handleTeamFull(const net::TEAM_FULL& msg) {
    std::println("[LobbyData] Team is full");
}
