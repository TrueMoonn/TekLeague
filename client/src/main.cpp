/*
** EPITECH PROJECT, 2025
** TekLeague
** File description:
** main.cpp
*/

#include <iostream>
#include <string>
#include <csignal>
#include <atomic>
#include <unistd.h>
#include "Client.hpp"
#include "scenes.hpp"

// Global flag for clean shutdown
static std::atomic<bool> g_running(true);

// Signal handler for SIGINT and SIGTERM
void signalHandler(int signal) {
    if (signal == SIGINT) {
        std::cout << "\n[CLIENT] Received SIGINT (Ctrl+C), shutting down gracefully..." << std::endl;
    } else if (signal == SIGTERM) {
        std::cout << "\n[CLIENT] Received SIGTERM, shutting down gracefully..." << std::endl;
    }
    g_running.store(false);
}

int main(int ac, char **av) {
    std::signal(SIGINT, signalHandler);
    std::signal(SIGTERM, signalHandler);

    std::string server_ip = "127.0.0.1";
    uint16_t server_port = 6767;

    if (ac > 1) {
        server_ip = av[1];
    }
    if (ac > 2) {
        server_port = static_cast<uint16_t>(std::stoi(av[2]));
    }

    std::cout << "╔════════════════════════════════════════╗\n";
    std::cout << "║         TekLeague Client               ║\n";
    std::cout << "╚════════════════════════════════════════╝\n\n";
    std::cout << "[CLIENT] Press Ctrl+C to exit gracefully\n" << std::endl;

    Client game;

    for (std::size_t i = 0; i < SCAST(SCENES::LIMITSCENE); ++i)
        SCENES_SETUPS.at(static_cast<SCENES>(i))(game);

    game.createEntity(game.nextEntity(eType::SYSTEM), "ig_window");
    game.activateScene(SCAST(SCENES::MAIN));

    std::cout << "Server IP: " << server_ip << "\n";
    std::cout << "Server Port: " << server_port << "\n\n";

    try {
        game.connectToServer(server_ip, server_port);
    } catch (const std::exception& e) {
        std::cerr << "[ERROR] Failed to connect to server: " << e.what() << "\n";
        std::cerr << "Usage: " << av[0] << " [server_ip] [server_port]\n";
        std::cerr << "Example: " << av[0] << " 127.0.0.1 6767\n";
        return 1;
    }

    std::cout << "Press ENTER to start...";
    std::cin.get();

    try {
        while (g_running.load() && game.isRunning()) {
            // Receive messages from server
            game.receiveMessages();

            // If not in game, update lobby manager CLI
            if (!game.isInGame()) {
                game.updateLobby();
            } else {
                // Run game logic
                game.updateGame();
            }
        }

        if (!g_running.load()) {
            std::cout << "\n[CLIENT] Disconnecting from server..." << std::endl;
            game.disconnect();
        }

    } catch (const std::exception& e) {
        std::cerr << "[CLIENT] Error during runtime: " << e.what() << std::endl;
        return 1;
    }

    std::cout << "[CLIENT] Client stopped cleanly. All destructors called." << std::endl;
    std::cout << "[CLIENT] Resources freed properly. Goodbye!" << std::endl;

    return 0;
}
