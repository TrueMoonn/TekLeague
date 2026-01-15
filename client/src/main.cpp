/*
** EPITECH PROJECT, 2025
** TekLeague
** File description:
** main.cpp
*/

#include <print>
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
        std::println("\n[CLIENT] Received SIGINT (Ctrl+C), shutting down gracefully...");
    } else if (signal == SIGTERM) {
        std::println("\n[CLIENT] Received SIGTERM, shutting down gracefully...");
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

    std::print("╔════════════════════════════════════════╗\n");
    std::print("║         TekLeague Client               ║\n");
    std::print("╚════════════════════════════════════════╝\n\n");
    std::println("[CLIENT] Press Ctrl+C to exit gracefully\n");

    Client game;

    for (std::size_t i = 0; i < SCAST(SCENES::LIMITSCENE); ++i)
        SCENES_SETUPS.at(static_cast<SCENES>(i))(game);

    game.createEntity(game.nextEntity(eType::SYSTEM), "ig_window");

    game.updateScene(te::sStatus::ACTIVATE, SCAST(SCENES::MAIN));
    std::print("Server IP: {}\n", server_ip);
    std::print("Server Port: {}\n\n", server_port);

    try {
        game.connectToServer(server_ip, server_port);
    } catch (const std::exception& e) {
        std::print(stderr, "[ERROR] Failed to connect to server: {}\n", e.what());
        std::print(stderr, "Usage: {} [server_ip] [server_port]\n", av[0]);
        std::print(stderr, "Example: {} 127.0.0.1 6767\n", av[0]);
        return 1;
    }

    std::print("[CLIENT] Starting game loop...\n");

    try {
        while (g_running.load() && game.isRunning()) {
            game.receiveMessages();
            game.updateGame();
        }

        if (!g_running.load()) {
            std::println("\n[CLIENT] Disconnecting from server...");
            game.disconnect();
        }

    } catch (const std::exception& e) {
        std::println(stderr, "[CLIENT] Error during runtime: {}", e.what());
        return 1;
    }

    std::println("[CLIENT] Client stopped cleanly. All destructors called.");
    std::println("[CLIENT] Resources freed properly. Goodbye!");

    return 0;
}
