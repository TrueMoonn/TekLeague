/*
** EPITECH PROJECT, 2025
** TekLeague
** File description:
** LobbyContext.hpp
*/


#pragma once

    #include <optional>

    #include <Network/generated_messages.hpp>
    #include <Network/Address.hpp>

    #include "clock.hpp"
    #include "Lobby.hpp"

class LobbyContext {
 public:
    ////// Timestamp default latency in seconds //////
    static constexpr float PLAYERS_UPDATES_DEFAULT_LATENCY = 1.0f / 60.0f;

 public:
    LobbyContext(uint max_players, const std::string& code);

    void run();
    const std::string& getCode();

    Lobby& getLobby();

    ////// Network //////

    bool addClient(const net::Address& address, uint32_t client_id);
    void removeClient(uint32_t client_id);
    const std::unordered_map<uint32_t, net::Address>& getClients() const;
    bool isFull() const;

    /**
     * @brief Try to get player updates if the timestamp delay has passed
     * @return std::optional containing the message if ready, std::nullopt otherwise
     */
    std::optional<net::PLAYERS_UPDATES> tryGetPlayerUpdates();


    ////// setters //////

    /**
     * @brief Set how often to send player updates (in seconds)
     */
    void setPlayersUpdateFrequency(float freq) {
        players_update.delay = size_t(SEC_TO_MICRO(freq));
    }

 private:
    Lobby lobby;

    ////// Network //////
    std::unordered_map<uint32_t, net::Address> connected_players;
    uint max_clients;

    te::Timestamp players_update = te::Timestamp(PLAYERS_UPDATES_DEFAULT_LATENCY);

    /**
     * @brief Build the PLAYERS_UPDATES message from the lobby's registry
     * @return The constructed message
     */
    net::PLAYERS_UPDATES getPlayerUpdates();

    /**
     * @brief Build the BUILDING_UPDATES message from the lobby's registry
     * @return The constructed message
     */
    net::BUILDINGS_UPDATES getBuildingsUpdates();

    /**
     * @brief Build the CREATURES_UPDATES message from the lobby's registry
     * @return The constructed message
     */
    net::CREATURES_UPDATES getCreaturesUpdates();

    /**
     * @brief Build the PROJECTILES_UPDATES message from the lobby's registry
     * @return The constructed message
     */
    net::PROJECTILES_UPDATES getProjectilesUpdates();

    /**
     * @brief Build the COLLECTIBLES_UPDATES message from the lobby's registry
     * @return The constructed message
     */
    net::COLLECTIBLES_UPDATES getCollectiblesUpdates();
};
