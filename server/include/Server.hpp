/*
** EPITECH PROJECT, 2025
** TekLeague
** File description:
** Server.hpp
*/

#pragma once

    #include <cstdint>
    #include <string>
    #include <unordered_map>
    #include <vector>

    #include <network/GameServer.hpp>
    #include <Network/ProtocolManager.hpp>
    #include "lobby/LobbyContext.hpp"

class Server : public te::network::GameServer {
 public:
    explicit Server(uint16_t port = 6767, const std::string& protocol = "UDP");

    void run();

    void sendAutomatic();

    /**
     * @brief Set how often to send PLAYERS_UPDATES (in seconds)
     */
    void setPlayersUpdateFrequency(std::size_t lobby_id, float frequency) {
        lobbies.at(lobby_id).setPlayersUpdateFrequency(frequency);
    }

 private:
    bool setPacketsHandlers();

    ////// Lobbies //////
    std::unordered_map<uint, LobbyContext> lobbies;
    std::unordered_map<std::string, uint> lobby_codes;
    uint next_lobby_id = 0;
    void createLobby(uint max_clients);
    void destroyLobby(uint lobby_id);
    void broadcastToLobby(uint lobby_id, const std::vector<uint8_t>& data);

    /**
     * @brief Generate a unique 6-character alphanumeric lobby code
     * @return A unique lobby code that doesn't exist yet
     */
    std::string generateUniqueLobbyCode();

    ////// Handlers //////
    void handleConnectionRequest(const std::vector<uint8_t>& data,
        const net::Address& sender);

    ////// Senders //////
    void sendPlayersUpdate();

};
