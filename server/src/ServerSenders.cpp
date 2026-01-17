/*
** EPITECH PROJECT, 2025
** TekLeague
** File description:
** Server.cpp
*/

#include <cstdint>
#include <string>
#include <vector>
#include <random>
#include <thread>
#include <chrono>
#include <print>
#include <algorithm>

#include <ECS/DenseSA.hpp>
#include <ECS/DenseZipper.hpp>

#include <Network/generated_messages.hpp>

#include <physic/components/position.hpp>
#include <entity_spec/components/health.hpp>
#include <entity_spec/components/team.hpp>
#include <interaction/components/player.hpp>

#include <network1/GameServer.hpp>

#include "Server.hpp"
#include "LobbyContext.hpp"

void Server::sendAutomatic() {
    try {
        if (ping.checkDelay()) {
            for (const auto& client : clients) {
                sendPing(client.first, client.second.id);
            }
        }

        std::vector<std::pair<uint32_t, std::vector<uint8_t>>> ingame_updates;
        std::vector<uint32_t> pre_game_lobbies;

        {
            std::lock_guard<std::mutex> lock(lobbies_mutex);

            for (auto& [lobby_id, ctx] : lobbies) {
                if (ctx.getGameState() == LobbyGameState::PRE_GAME) {
                    if (ctx.shouldSendPlayersList()) {
                        pre_game_lobbies.push_back(lobby_id);
                    }
                } else if (ctx.getGameState() == LobbyGameState::IN_GAME) {
                    if (auto msg = ctx.tryGetPlayerUpdates())
                        ingame_updates.emplace_back(lobby_id, msg->serialize());
                    if (auto msg = ctx.tryGetBuildingsUpdates())
                        ingame_updates.emplace_back(lobby_id, msg->serialize());
                    if (auto msg = ctx.tryGetCreaturesUpdates())
                        ingame_updates.emplace_back(lobby_id, msg->serialize());
                    if (auto msg = ctx.tryGetProjectilesUpdates())
                        ingame_updates.emplace_back(lobby_id, msg->serialize());
                    if (auto msg = ctx.tryGetCollectiblesUpdates())
                        ingame_updates.emplace_back(lobby_id, msg->serialize());
                    if (auto msg = ctx.tryGetInventoriesUpdates())
                        ingame_updates.emplace_back(lobby_id, msg->serialize());
                    if (auto msg = ctx.tryGetStatsUpdates())
                        ingame_updates.emplace_back(lobby_id, msg->serialize());
                    if (auto msg = ctx.tryGetScore())
                        ingame_updates.emplace_back(lobby_id, msg->serialize());
                    if (auto msg = ctx.tryGetGameDuration())
                        ingame_updates.emplace_back(lobby_id, msg->serialize());
                    if (auto msg = ctx.tryGetScoreboard())
                        ingame_updates.emplace_back(lobby_id, msg->serialize());
                }
            }

            for (uint32_t lobby_id : pre_game_lobbies) {
                sendPlayersListUnsafe(lobby_id);
            }

            if (lobbies_list_timestamp.checkDelay()) {
                sendLobbiesListUnsafe();
            }
        }

        for (const auto& [lobby_id, data] : ingame_updates) {
            broadcastToLobby(lobby_id, data);
        }
    } catch (const std::exception& e) {
        std::println(stderr, "[Server::sendAutomatic] ERROR: {}", e.what());
    }
}

void Server::sendPing(const net::Address& address, uint32_t client_id) {
    net::PING msg;
    sendTo(address, msg.serialize());
}

void Server::sendPong(const net::Address& address, uint32_t client_id) {
    net::PONG msg;
    sendTo(address, msg.serialize());
}

void Server::sendLoggedIn(const net::Address& address, uint32_t client_id) {
    net::LOGGED_IN msg;
    msg.id = client_id;
    sendTo(address, msg.serialize());
}

void Server::sendLoggedOut(const net::Address& address, uint32_t client_id) {
    net::LOGGED_OUT msg;
    msg.id = client_id;
    sendTo(address, msg.serialize());
}

void Server::sendUsernameAlreadyTaken(const net::Address& address) {
    net::USERNAME_ALREADY_TAKEN msg;
    sendTo(address, msg.serialize());
}

void Server::sendLobbyJoined(const net::Address& address, uint32_t client_id) {
    net::LOBBY_JOINED msg;
    msg.id = client_id;
    sendTo(address, msg.serialize());
}

void Server::sendLobbyCreated(
    const net::Address& address, const std::string& lobby_code) {
    net::LOBBY_CREATED msg;
    std::memcpy(msg.lobby_code, lobby_code.c_str(),
        std::min(lobby_code.size(), size_t(6)));
    sendTo(address, msg.serialize());
}

void Server::sendLobbiesList(const net::Address& address) {
    net::LOBBIES_LIST msg;

    {
        std::lock_guard<std::mutex> lock(lobbies_mutex);
        for (uint32_t lobby_id : public_lobbies) {
            if (lobbies.find(lobby_id) != lobbies.end()) {
                msg.lobby_codes.push_back(lobbies.at(lobby_id).getCode());
            }
        }
    }

    sendTo(address, msg.serialize());
}

void Server::sendLobbiesListUnsafe() {
    net::LOBBIES_LIST msg;

    for (uint32_t lobby_id : public_lobbies) {
        if (lobbies.find(lobby_id) != lobbies.end()) {
            msg.lobby_codes.push_back(lobbies.at(lobby_id).getCode());
        }
    }

    auto serialized = msg.serialize();
    for (const auto& [address, client] : clients) {
        if (!client.in_lobby) {
            sendTo(address, serialized);
        }
    }

    std::println("[Server::sendLobbiesListUnsafe] Sent to all non-lobby "
        "clients ({} lobbies)", msg.lobby_codes.size());
}

void Server::sendGameStarting(uint32_t lobby_id) {
    std::println(
        "[Server] sendGameStarting: Broadcasting GAME_STARTING to lobby {}",
        lobby_id);
    net::GAME_STARTING msg;
    auto serialized = msg.serialize();
    broadcastToLobby(lobby_id, serialized);
    std::println("[Server] sendGameStarting: Broadcast complete");
}

void Server::sendPlayersListUnsafe(uint32_t lobby_id) {
    if (lobbies.find(lobby_id) == lobbies.end()) {
        std::println("[Server::sendPlayersListUnsafe] Lobby not found");
        return;
    }

    net::PLAYERS_LIST msg;

    for (const auto& [client_id, address] : lobbies.at(lobby_id).getClients()) {
        auto client_opt = getClient(address);
        if (client_opt) {
            auto& client = client_opt->get();
            net::PlayerListEntry entry;
            entry.id = client.id;
            entry.is_admin = isAdmin(address, lobby_id) ? 1 : 0;
            entry.team = client.team;
            std::memset(entry.username, 0, 32);
            std::memcpy(entry.username, client.username.c_str(),
                std::min(client.username.size(), size_t(32)));
            msg.players.push_back(entry);
        }
    }

    std::println(
        "[Server::sendPlayersListUnsafe] Broadcasting to lobby with {} players",
        msg.players.size());
    broadcastToLobbyUnsafe(lobby_id, msg.serialize());
}

void Server::sendPlayersList(uint32_t lobby_id) {
    std::println("[Server::sendPlayersList] Entering for lobby {}", lobby_id);
    std::lock_guard<std::mutex> lock(lobbies_mutex);
    std::println("[Server::sendPlayersList] Lock acquired");

    sendPlayersListUnsafe(lobby_id);

    std::println("[Server::sendPlayersList] Completed");
}

void Server::sendLobbyVisibilityChanged(uint32_t lobby_id, bool is_public) {
    net::LOBBY_VISIBILITY_CHANGED msg;
    msg.is_public = is_public ? 1 : 0;
    broadcastToLobby(lobby_id, msg.serialize());
}

void Server::sendLobbyDestroyed(uint32_t lobby_id) {
    net::LOBBY_DESTROYED msg;
    broadcastToLobby(lobby_id, msg.serialize());
}

void Server::sendLobbyFull(const net::Address& address) {
    net::LOBBY_FULL msg;
    sendTo(address, msg.serialize());
}

void Server::sendBadLobbyCode(const net::Address& address) {
    net::BAD_LOBBY_CODE msg;
    sendTo(address, msg.serialize());
}

void Server::sendNotAdmin(const net::Address& address) {
    net::NOT_ADMIN msg;
    sendTo(address, msg.serialize());
}

void Server::sendTeamFull(const net::Address& address) {
    net::TEAM_FULL msg;
    sendTo(address, msg.serialize());
}

void Server::sendAdminGamePaused(uint32_t lobby_id) {
    net::ADMIN_GAME_PAUSED msg;
    broadcastToLobby(lobby_id, msg.serialize());
}

void Server::sendGameEnded(uint32_t lobby_id) {
    std::println(
        "[Server] sendGameEnded: Broadcasting GAME_END to lobby {}",
        lobby_id);
    net::GAME_END msg;
    msg.winning_team = 0;  // TODO(x): Determine winning team
    auto serialized = msg.serialize();
    broadcastToLobby(lobby_id, serialized);
    std::println("[Server] sendGameEnded: Broadcast complete");
}

void Server::sendPlayersInit(uint32_t lobby_id) {
    {
        std::lock_guard<std::mutex> lock(lobbies_mutex);

        for (auto& [ctx_id, ctx] : lobbies) {
            if (ctx_id == lobby_id) {
                net::PLAYERS_INIT msg = ctx.getPlayersInit();
                broadcastToLobbyUnsafe(lobby_id, msg.serialize());
                return;
            }
        }
    }
}


void Server::sendBuildingsInit(uint32_t lobby_id) {
    {
        std::lock_guard<std::mutex> lock(lobbies_mutex);

        for (auto& [ctx_id, ctx] : lobbies) {
            if (ctx_id == lobby_id) {
                net::BUILDINGS_INIT msg = ctx.getBuildingsInit();
                broadcastToLobbyUnsafe(lobby_id, msg.serialize());
                return;
            }
        }
    }
}
