/*
** EPITECH PROJECT, 2025
** TekLeague
** File description:
** Server.hpp
*/

#pragma once

    #include <cstdint>
    #include <string>
    #include <vector>

    #include <network/GameServer.hpp>
    #include <Network/ProtocolManager.hpp>
    #include "Game.hpp"
    #include "clock.hpp"

class Server : public te::network::GameServer, public Game {
 public:
    ////// Timestamp default latency in seconds //////
    static constexpr float PLAYERS_UPDATES_DEFAULT_LATENCY = 1.0f / 60.0f;

 public:
    explicit Server(uint16_t port = 6767, const std::string& protocol = "UDP");

    void run() override;

    void sendAutomatic();

    /**
     * @brief Set how often to send PLAYERS_UPDATES (in seconds)
     */
    void setPlayersUpdateFrequency(float frequency) {
        players_update_interval = frequency;
        players_update_timestamp.delay = size_t(SEC_TO_MICRO(frequency));
    }

 private:
    bool setPacketsHandlers();

    std::uint32_t next_player_id;

    ////// Timestamp //////
    te::Timestamp players_update_timestamp = te::Timestamp(PLAYERS_UPDATES_DEFAULT_LATENCY);
    float players_update_interval = PLAYERS_UPDATES_DEFAULT_LATENCY;

    ////// Handlers //////
    void handleConnectionRequest(const std::vector<uint8_t>& data,
        const net::Address& sender);

    ////// Senders //////
    void sendPlayersUpdate();

};
