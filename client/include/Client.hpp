/*
** EPITECH PROJECT, 2025
** TekLeague
** File description:
** Client.hpp
*/

#pragma once

    #include "GameTool.hpp"
    #include "LobbyContext.hpp"
    #include <network/GameClient.hpp>
    #include <clock.hpp>
    #include <ECS/Entity.hpp>
    #include <unordered_map>
    #include "entities.hpp"

    #define FRAME_LIMIT 1.0f / 60   // 60 fps

class Client : public te::GameTool, public te::network::GameClient {
 public:
    static constexpr const char* CLIENT_PLUGINS_PATH = "client/plugins";
    static constexpr const char* PROTOCOL_PATH = "config/protocol.json";
    static constexpr const char* COM_DEFAULT_MODE = "UDP";

 public:
    Client();
    ~Client();

    void run();

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
    bool isInGame() const { return _lobby_data.isInGame(); }

    /**
     * @brief Receive messages from server
     */
    void receiveMessages();

    /**
     * @brief Update game logic
     */
    void updateGame();

    /**
     * @brief Get lobby data manager
     */
    LobbyContext& getLobbyData() { return _lobby_data; }

    /**
     * @brief Get next entity ID for a given type
     */
    ECS::Entity nextEntity(eType type);

    std::string client_name = "default";

 private:
    bool _running = true;
    te::Timestamp _framelimit{FRAME_LIMIT};
    LobbyContext _lobby_data;
    std::unordered_map<eType, ECS::Entity> _nextEntities;

    void registerMessageHandlers();

    void handlePing();
    void handlePong();
    void sendPing();
    void sendPong();
};
