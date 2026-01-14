/*
** EPITECH PROJECT, 2025
** TekLeague
** File description:
** LobbyManager.cpp
*/

#include "LobbyManager.hpp"
#include <iostream>
#include <cstring>
#include <algorithm>
#include <chrono>
#include <thread>
#include <limits>

LobbyManager::LobbyManager() {
    _state = State::DISCONNECTED;
}

void LobbyManager::setSendFunction(std::function<void(const std::vector<uint8_t>&)> sendFunc) {
    _sendFunc = sendFunc;
}

void LobbyManager::setServerAddress(const net::Address& address) {
    _server_address = address;
    changeState(State::LOBBY_MENU);
}

void LobbyManager::update() {
    switch (_state) {
        case State::DISCONNECTED:
            std::cout << "\n[!] Not connected to server. Waiting for connection...\n";
            break;

        case State::LOBBY_MENU:
            showMainMenu();
            break;

        case State::IN_LOBBY:
            showLobbyMenu();
            break;

        case State::LOGGING_IN:
        case State::CREATING_LOBBY:
        case State::JOINING_LOBBY:
        case State::FETCHING_LOBBIES:
        case State::WAITING_GAME_START:
        case State::WAITING_VISIBILITY:
        case State::WAITING_PAUSE:
            // Waiting for server response, do nothing
            break;

        case State::IN_GAME:
            // Game is running, CLI not active
            break;
    }
}

void LobbyManager::changeState(State new_state) {
    _state = new_state;
}

// ==================== CLI DISPLAY ====================

void LobbyManager::clearScreen() {
    // Temporarily disabled to see debug logs
    // std::cout << "\033[2J\033[1;1H"; // ANSI escape codes
}

void LobbyManager::showMainMenu() {
    // clearScreen(); // Temporarily disabled
    std::cout << "\n========================================\n";
    std::cout << "╔════════════════════════════════════════╗\n";
    std::cout << "║         TekLeague - Lobby Menu         ║\n";
    std::cout << "╚════════════════════════════════════════╝\n\n";

    if (!_username.empty()) {
        std::cout << "Logged in as: " << _username << " (ID: " << _client_id << ")\n\n";
    }

    std::cout << "1. Login\n";
    std::cout << "2. Create Lobby\n";
    std::cout << "3. Join Lobby\n";
    std::cout << "4. List Public Lobbies\n";
    std::cout << "5. Logout\n";
    std::cout << "6. Quit\n";
    std::cout << "\nChoice: ";

    std::string choice;
    std::getline(std::cin, choice);

    if (choice == "1") {
        promptLogin();
    } else if (choice == "2") {
        if (_username.empty()) {
            std::cout << "\n[!] You must login first!\n";
            std::this_thread::sleep_for(std::chrono::milliseconds(1000));
            return;
        }
        sendCreateLobby();
        changeState(State::CREATING_LOBBY);
        std::cout << "\n[+] Creating lobby...\n";
    } else if (choice == "3") {
        if (_username.empty()) {
            std::cout << "\n[!] You must login first!\n";
            std::this_thread::sleep_for(std::chrono::milliseconds(1000));
            return;
        }
        promptJoinLobby();
    } else if (choice == "4") {
        sendGetPublicLobbies();
        changeState(State::FETCHING_LOBBIES);
        std::cout << "\n[+] Fetching public lobbies...\n";
    } else if (choice == "5") {
        if (!_username.empty()) {
            sendLogout();
            std::cout << "\n[+] Logging out...\n";
        } else {
            std::cout << "\n[!] Not logged in.\n";
        }
        std::this_thread::sleep_for(std::chrono::milliseconds(500));
    } else if (choice == "6") {
        std::cout << "\n[!] Exiting...\n";
        exit(0);
    }
}

void LobbyManager::showLobbyMenu() {
    // clearScreen(); // Temporarily disabled
    std::cout << "\n========================================\n";
    std::cout << "╔════════════════════════════════════════╗\n";
    std::cout << "║           Lobby: " << _lobby_code << "              ║\n";
    std::cout << "╚════════════════════════════════════════╝\n\n";

    std::cout << "Username: " << _username << " (ID: " << _client_id << ")\n";
    std::cout << "Role: " << (_is_admin ? "ADMIN" : "Player") << "\n";
    std::cout << "Visibility: " << (_lobby_is_public ? "PUBLIC" : "PRIVATE") << "\n\n";

    displayPlayers();

    std::cout << "\n--- Commands ---\n";
    if (_is_admin) {
        std::cout << "1. Start Game\n";
        std::cout << "2. Toggle Visibility (Public/Private)\n";
        std::cout << "3. Pause/Unpause Game\n";
    }
    std::cout << "4. Leave Lobby\n";
    std::cout << "5. Refresh\n";
    std::cout << "\nChoice: ";

    std::string choice;
    std::getline(std::cin, choice);

    if (choice == "1" && _is_admin) {
        sendStartGame();
        changeState(State::WAITING_GAME_START);
        std::cout << "\n[+] Starting game...\n";
        std::this_thread::sleep_for(std::chrono::milliseconds(200));
    } else if (choice == "2" && _is_admin) {
        sendToggleVisibility();
        changeState(State::WAITING_VISIBILITY);
        std::cout << "\n[+] Toggling lobby visibility...\n";
        std::this_thread::sleep_for(std::chrono::milliseconds(200));
    } else if (choice == "3" && _is_admin) {
        sendPauseGame();
        changeState(State::WAITING_PAUSE);
        std::cout << "\n[+] Toggling pause...\n";
        std::this_thread::sleep_for(std::chrono::milliseconds(200));
    } else if (choice == "4") {
        sendLeaveLobby();
        _lobby_code.clear();
        _is_admin = false;
        _players_in_lobby.clear();
        changeState(State::LOBBY_MENU);
        std::cout << "\n[+] Left lobby.\n";
        std::cin.get();
    } else if (choice == "5") {
        // Just refresh
    } else if (!_is_admin && (choice == "1" || choice == "2" || choice == "3")) {
        std::cout << "\n[!] Only admin can do that!\n";
        std::this_thread::sleep_for(std::chrono::milliseconds(1000));
    }
}

void LobbyManager::promptLogin() {
    std::cout << "\nEnter username (max 32 chars, alphanumeric): ";
    std::string username;
    std::getline(std::cin, username);

    if (username.empty() || username.length() > 32) {
        std::cout << "[!] Invalid username length.\n";
        std::this_thread::sleep_for(std::chrono::milliseconds(1000));
        return;
    }

    _username = username;
    sendLogin(username);
    changeState(State::LOGGING_IN);
    std::cout << "[+] Logging in as " << username << "...\n";
}

void LobbyManager::promptJoinLobby() {
    std::cout << "\nEnter lobby code (6 characters): ";
    std::string code;
    std::getline(std::cin, code);

    if (code.length() != 6) {
        std::cout << "[!] Lobby code must be 6 characters.\n";
        std::this_thread::sleep_for(std::chrono::milliseconds(1000));
        return;
    }

    _lobby_code = code;
    std::cout << "[LobbyManager::promptJoinLobby] Saved lobby code: '" << _lobby_code << "'\n";

    sendJoinLobby(code);
    changeState(State::JOINING_LOBBY);
    std::cout << "[+] Joining lobby " << code << "...\n";
}

void LobbyManager::displayPlayers() {
    std::cout << "Players in lobby (" << _players_in_lobby.size() << "):\n";
    for (const auto& player : _players_in_lobby) {
        std::string name(player.username, strnlen(player.username, 32));
        std::cout << "  - " << name << " (ID: " << player.id << ")";
        if (player.id == _client_id) {
            std::cout << " [YOU]";
        }
        std::cout << "\n";
    }
}

std::string LobbyManager::readLine() {
    std::string line;
    std::getline(std::cin, line);
    return line;
}

void LobbyManager::sendLogin(const std::string& username) {
    net::LOGIN msg;
    std::memset(msg.username, 0, 32);
    std::memcpy(msg.username, username.c_str(), std::min(username.size(), size_t(32)));

    if (_sendFunc) {
        _sendFunc(msg.serialize());
    }
}

void LobbyManager::sendLogout() {
    net::LOGOUT msg;
    if (_sendFunc) {
        _sendFunc(msg.serialize());
    }
}

void LobbyManager::sendCreateLobby() {
    net::CREATE_LOBBY msg;
    if (_sendFunc) {
        _sendFunc(msg.serialize());
    }
}

void LobbyManager::sendJoinLobby(const std::string& code) {
    net::JOIN_LOBBY msg;
    std::memset(msg.lobby_code, 0, 6);
    std::memcpy(msg.lobby_code, code.c_str(), std::min(code.size(), size_t(6)));

    if (_sendFunc) {
        _sendFunc(msg.serialize());
    }
}

void LobbyManager::sendGetPublicLobbies() {
    net::GET_ALL_PUBLIC_LOBBIES msg;
    if (_sendFunc) {
        _sendFunc(msg.serialize());
    }
}

void LobbyManager::sendLeaveLobby() {
    net::LEAVE_LOBBY msg;
    if (_sendFunc) {
        _sendFunc(msg.serialize());
    }
}

void LobbyManager::sendStartGame() {
    net::ADMIN_START_GAME msg;
    if (_sendFunc) {
        _sendFunc(msg.serialize());
    }
}

void LobbyManager::sendToggleVisibility() {
    net::TOGGLE_LOBBY_PRIVATE_PUBLIC msg;
    if (_sendFunc) {
        _sendFunc(msg.serialize());
    }
}

void LobbyManager::sendPauseGame() {
    net::ADMIN_PAUSE_GAME msg;
    if (_sendFunc) {
        _sendFunc(msg.serialize());
    }
}

// ==================== MESSAGE HANDLERS ====================

void LobbyManager::handleLoggedIn(const net::LOGGED_IN& msg) {
    _client_id = msg.id;
    changeState(State::LOBBY_MENU);

    std::cout << "\n[Y] Logged in successfully! Client ID: " << _client_id << "\n";
}

void LobbyManager::handleLoggedOut(const net::LOGGED_OUT& msg) {
    std::cout << "\n[Y] Logged out successfully.\n";
    _username.clear();
    _client_id = 0;
    changeState(State::LOBBY_MENU);
}

void LobbyManager::handleUsernameAlreadyTaken(const net::USERNAME_ALREADY_TAKEN& msg) {
    std::cout << "\n[N] Username already taken! Please choose another.\n";
    changeState(State::LOBBY_MENU);
}

void LobbyManager::handleLobbyCreated(const net::LOBBY_CREATED& msg) {
    _lobby_code = std::string(msg.lobby_code, 6);
    _is_admin = true;
    changeState(State::IN_LOBBY);

    std::cout << "\n[Y] Lobby created! Code: " << _lobby_code << "\n";
    std::cout << "You are the ADMIN. Share this code with friends!\n";
}

void LobbyManager::handleLobbyJoined(const net::LOBBY_JOINED& msg) {
    std::cout << "\n[LobbyManager::handleLobbyJoined] Received LOBBY_JOINED packet\n";
    std::cout << "[LobbyManager::handleLobbyJoined] Client ID: " << msg.id << "\n";
    std::cout << "[LobbyManager::handleLobbyJoined] Current lobby code: '" << _lobby_code << "'\n";

    changeState(State::IN_LOBBY);

    std::cout << "[Y] Successfully joined lobby!\n";
    std::cout << "[DEBUG] Waiting for PLAYERS_LIST packet...\n";
}

void LobbyManager::handleBadLobbyCode(const net::BAD_LOBBY_CODE& msg) {
    std::cout << "\n[N] Invalid lobby code! Please check and try again.\n";
    changeState(State::LOBBY_MENU);
}

void LobbyManager::handleLobbyFull(const net::LOBBY_FULL& msg) {
    std::cout << "\n[N] Lobby is full! Cannot join.\n";
    changeState(State::LOBBY_MENU);
}

void LobbyManager::handlePlayersList(const net::PLAYERS_LIST& msg) {
    std::cout << "\n[LobbyManager::handlePlayersList] Received PLAYERS_LIST packet\n";
    std::cout << "[LobbyManager::handlePlayersList] Number of players: " << msg.players.size() << "\n";

    _players_in_lobby = msg.players;

    // Check if we're admin (first player in created lobby)
    if (!_players_in_lobby.empty() && _players_in_lobby[0].id == _client_id) {
        _is_admin = true;
        std::cout << "[LobbyManager::handlePlayersList] I am the admin\n";
    }

    std::cout << "[Y] Players list updated (" << _players_in_lobby.size() << " players)\n";

    // Debug: print all players
    for (size_t i = 0; i < _players_in_lobby.size(); ++i) {
        const auto& player = _players_in_lobby[i];
        std::string name(player.username, strnlen(player.username, 32));
        std::cout << "  [" << i << "] ID=" << player.id << " Username='" << name << "'"
                  << (player.id == _client_id ? " [ME]" : "") << "\n";
    }
}

void LobbyManager::handleLobbiesList(const net::LOBBIES_LIST& msg) {
    std::cout << "\n========================================\n";
    std::cout << "╔════════════════════════════════════════╗\n";
    std::cout << "║         Public Lobbies List            ║\n";
    std::cout << "╚════════════════════════════════════════╝\n\n";

    if (msg.lobby_codes.empty()) {
        std::cout << "No public lobbies available.\n";
    } else {
        std::cout << "Available lobbies:\n";
        for (const auto& code : msg.lobby_codes) {
            std::cout << "  - " << code << "\n";
        }
    }

    changeState(State::LOBBY_MENU);
}

void LobbyManager::handleLobbyVisibilityChanged(const net::LOBBY_VISIBILITY_CHANGED& msg) {
    _lobby_is_public = (msg.is_public != 0);
    std::cout << "\n========================================\n";
    std::cout << "[Y] Lobby visibility changed!\n";
    std::cout << "    Status: " << (_lobby_is_public ? "PUBLIC" : "PRIVATE") << "\n";
    std::cout << "========================================\n";

    changeState(State::IN_LOBBY);
}

void LobbyManager::handleGameStarting(const net::GAME_STARTING& msg) {
    std::cout << "\n========================================\n";
    std::cout << "╔════════════════════════════════════════╗\n";
    std::cout << "║          GAME STARTING!                ║\n";
    std::cout << "╚════════════════════════════════════════╝\n";
    std::cout << "========================================\n";
    std::cout << "Loading game...\n";

    changeState(State::IN_GAME);
}

void LobbyManager::handleLobbyDestroyed(const net::LOBBY_DESTROYED& msg) {
    std::cout << "\n[!] Lobby has been destroyed by the host.\n";
    _lobby_code.clear();
    _is_admin = false;
    _players_in_lobby.clear();
    changeState(State::LOBBY_MENU);
}

void LobbyManager::handleNotAdmin(const net::NOT_ADMIN& msg) {
    std::cout << "\n[N] You don't have admin permissions for this action!\n";
}

void LobbyManager::handleAdminGamePaused(const net::ADMIN_GAME_PAUSED& msg) {
    std::cout << "\n========================================\n";
    std::cout << "[Y] Game pause toggled!\n";
    std::cout << "========================================\n";

    changeState(State::IN_LOBBY);
}
