/*
** EPITECH PROJECT, 2025
** TekLeague
** File description:
** Client.hpp
*/

#pragma once

    #include "Game.hpp"
    #include "LobbyManager.hpp"
    #include <network/GameClient.hpp>

class Client : public Game, public te::network::GameClient {
 public:
    static constexpr const char* CLIENT_PLUGINS_PATH = "client/plugins";
    static constexpr const char* PROTOCOL_PATH = "config/protocol.json";
    static constexpr const char* COM_DEFAULT_MODE = "UDP";
 public:
    Client();
    ~Client();

    void run() override;

    /**
     * @brief Connect to server
     */
    void connectToServer(const std::string& ip, uint16_t port);

    /**
     * @brief Disconnect from server
     */
    void disconnect();

    /**
     * @brief Check if client is running
     */
    bool isRunning() const { return _running; }

    /**
     * @brief Check if in game (not in lobby)
     */
    bool isInGame() const { return _lobby_manager.isInGame(); }

    /**
     * @brief Receive messages from server
     */
    void receiveMessages();

    /**
     * @brief Update lobby manager
     */
    void updateLobby();

    /**
     * @brief Update game logic
     */
    void updateGame();

    /**
     * @brief Get lobby manager
     */
    LobbyManager& getLobbyManager() { return _lobby_manager; }

 private:
    LobbyManager _lobby_manager;

    void registerMessageHandlers();
};
