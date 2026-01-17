/*
** EPITECH PROJECT, 2025
** GameTwo
** File description:
** main.cpp
*/

#include <string>
#include <csignal>
#include <print>
#include <cstdio>
#define WIN32_LEAN_AND_MEAN
#define _WINSOCK_DEPRECATED_NO_WARNINGS
#ifndef _WIN32
    #include <unistd.h>
#endif
#include <cstring>
#include <optional>
#include <functional>

#include "Server.hpp"


std::optional<std::reference_wrapper<Server>> g_server;

// Signal handler for SIGINT and SIGTERM
void signalHandler(int signal) {
    const char* msg = nullptr;
    if (signal == SIGINT) {
        msg =
        "\n[SERVER] Received SIGINT (Ctrl+C), shutting down gracefully...\n";
    } else if (signal == SIGTERM) {
        msg = "\n[SERVER] Received SIGTERM, shutting down gracefully...\n";
    }

    if (msg) {
#ifndef _WIN32
        write(STDOUT_FILENO, msg, strlen(msg));
#else
        fprintf(stdout, "%s", msg);
        fflush(stdout);
#endif
    }

    if (g_server.has_value()) {
        g_server->get().requestStop();
    }
}

int main(int ac, char **av) {
    uint16_t port = 6767;
    std::string protocol = "UDP";

    if (ac > 1) {
        port = static_cast<uint16_t>(std::stoi(av[1]));
    }
    if (ac > 2) {
        protocol = av[2];
    }

    std::print("╔════════════════════════════════════════╗\n");
    std::print("║         TekLeague Server               ║\n");
    std::print("╚════════════════════════════════════════╝\n\n");
    std::println("[SERVER] Starting on port {} with protocol {}",
        port, protocol);
    std::println("[SERVER] Press Ctrl+C to stop gracefully\n");

    try {
        Server game(port, protocol);
        g_server = std::ref(game);

        // Setup signal handlers for clean shutdown
        std::signal(SIGINT, signalHandler);
        std::signal(SIGTERM, signalHandler);

        game.run();

        game.stop();
    } catch (const std::exception& e) {
        std::println(stderr, "[SERVER] Fatal error: {}", e.what());
        g_server.reset();
        return 1;
    }

    g_server.reset();
    std::println("[SERVER] Server stopped cleanly. All destructors called.");
    std::println("[SERVER] Resources freed properly. Goodbye!");
    return 0;
}
