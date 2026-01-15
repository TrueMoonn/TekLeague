/*
** EPITECH PROJECT, 2025
** TekLeague
** File description:
** ClientHandlers.cpp
*/

#include <print>
#include <cstring>

#include <entity_spec/components/team.hpp>
#include <interaction/components/player.hpp>
#include <components/competences/target.hpp>
#include <physic/components/position.hpp>
#include <physic/components/velocity.hpp>

#include "Client.hpp"
#include "components/stats/health.hpp"
#include "components/stats/mana.hpp"
#include "components/stats/xp.hpp"
#include "my.hpp"

void Client::handleLoggedIn(const net::LOGGED_IN& msg) {
    _client_id = msg.id;
    std::println("[Client] Logged in with ID: {}", _client_id);
}

void Client::handleLoggedOut(const net::LOGGED_OUT& msg) {
    std::println("[Client] Logged out");
    _username.clear();
    _client_id = 0;
}

void Client::handleUsernameAlreadyTaken(const net::USERNAME_ALREADY_TAKEN& msg) {
    std::println("[Client] Username already taken");
    _username.clear();
}

void Client::handleLobbyCreated(const net::LOBBY_CREATED& msg) {
    setCode(std::string(msg.lobby_code, 6));
    _is_admin = true;
    std::println("[Client] Lobby created: {} (admin)", getCode());
}

void Client::handleLobbyJoined(const net::LOBBY_JOINED& msg) {
    std::println("[Client] Successfully joined lobby: {}", getCode());
}

void Client::handleBadLobbyCode(const net::BAD_LOBBY_CODE& msg) {
    std::println("[Client] Invalid lobby code");
    setCode("");
}

void Client::handleLobbyFull(const net::LOBBY_FULL& msg) {
    std::println("[Client] Lobby is full");
    setCode("");
}

void Client::handlePlayersList(const net::PLAYERS_LIST& msg) {
    setPlayers(msg.players);
    std::println("[Client] Players list updated ({} players)", getPlayers().size());

    _is_admin = false;
    for (const auto& player : getPlayers()) {
        if (player.id == _client_id && player.is_admin != 0) {
            _is_admin = true;
            break;
        }
    }

    for (size_t i = 0; i < getPlayers().size(); ++i) {
        const auto& player = getPlayers()[i];
        std::string name(player.username, strnlen(player.username, 32));
        std::println("  [{}] ID={} Team={} Username='{}'{}{}", i, player.id,
                static_cast<int>(player.team), name,
                (player.id == _client_id ? " [ME]" : ""),
                (player.is_admin != 0 ? " [ADMIN]" : ""));
    }
}

void Client::handleLobbiesList(const net::LOBBIES_LIST& msg) {
    _cached_lobbies_list = msg.lobby_codes;
    std::println("[Client] Received lobbies list ({} lobbies)", _cached_lobbies_list.size());

    for (const auto& code : _cached_lobbies_list) {
        std::println("  - {}", code);
    }
}

void Client::handleLobbyVisibilityChanged(const net::LOBBY_VISIBILITY_CHANGED& msg) {
    setPublic(msg.is_public != 0);
    std::println("[Client] Lobby visibility changed to: {}",
            (isPublic() ? "PUBLIC" : "PRIVATE"));
}

void Client::handleGameStarting(const net::GAME_STARTING& msg) {
    std::println("[Client] GAME STARTING!");
    setGameState(LobbyGameState::IN_GAME);
}

void Client::handleLobbyDestroyed(const net::LOBBY_DESTROYED& msg) {
    std::println("[Client] Lobby has been destroyed");
    setCode("");
    _is_admin = false;
    clearPlayers();
    setGameState(LobbyGameState::PRE_GAME);
}

void Client::handleNotAdmin(const net::NOT_ADMIN& msg) {
    std::println("[Client] You don't have admin permissions");
}

void Client::handleAdminGamePaused(const net::ADMIN_GAME_PAUSED& msg) {
    std::println("[Client] Game paused/resumed");
}

void Client::handleTeamFull(const net::TEAM_FULL& msg) {
    std::println("[Client] Team is full");
}

void Client::handlePing() {
    sendPong();
}

void Client::handlePong() {
    // TODO(PIERRE): si ca fait longtemps = deco
}

void Client::handlePlayersInit(const net::PLAYERS_INIT& msg) {
    auto& targets = getComponent<Target>();
    auto& teams = getComponent<addon::eSpec::Team>();
    for (auto& player : msg.players) {
        ECS::Entity e = player.entity;
        createEntity(e, CHAMPIONS[player.champ], {player.x, player.y});
        targets.getComponent(e).x = player.x;
        targets.getComponent(e).y = player.y;
        teams.getComponent(e).name = TEAMS[player.team];
        if (player.id == getClientId()) {
            createComponent<addon::intact::Player>(e);
        }
    }
}

void Client::handlePlayersUpdate(const net::PLAYERS_UPDATES& msg) {
    auto& pos = getComponent<addon::physic::Position2>();
    auto& vels = getComponent<addon::physic::Velocity2>();
    auto& targets = getComponent<Target>();
    auto& healths = getComponent<Health>();
    auto& xps = getComponent<Xp>();
    auto& manas = getComponent<Mana>();

    for (auto& player : msg.players) {
        ECS::Entity e = player.entity;
        targets.getComponent(e).x = player.direction_x;
        targets.getComponent(e).y = player.direction_y;
        vels.getComponent(e).x = player.vel_x;
        vels.getComponent(e).y = player.vel_y;
        pos.getComponent(e).x = player.x;
        pos.getComponent(e).y = player.y;
        healths.getComponent(e).amount = player.hp;
        xps.getComponent(e).amount = player.level;
        manas.getComponent(e).amount = player.mana;
    }
};
