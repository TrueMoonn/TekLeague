#include <vector>

#include <Network/generated_messages.hpp>
#include <Network/Address.hpp>
#include <ECS/DenseZipper.hpp>
#include <physic/components/position.hpp>
#include <entity_spec/components/health.hpp>
#include <interaction/components/player.hpp>

#include "lobby/LobbyContext.hpp"

net::PLAYERS_UPDATES LobbyContext::getPlayerUpdates() {
    net::PLAYERS_UPDATES msg;

    auto& registry = const_cast<ECS::Registry&>(lobby.getRegistry());
    auto& positions = registry.getComponents<addon::physic::Position2>();
    auto& healths = registry.getComponents<addon::eSpec::Health>();
    auto& players = registry.getComponents<addon::intact::Player>();

    for (auto&& [entity, player, pos, health] :
        ECS::IndexedDenseZipper(players, positions, healths)) {
        if (entity < eField::CHAMPION_BEGIN || entity > eField::CHAMPION_END)
            continue;

        net::PlayerUpdate state;
        state.id = static_cast<uint32_t>(entity),
        state.x = pos.x,
        state.y = pos.y,
        state.hp = static_cast<double>(health.amount),
        // TODO(xxx): implement later (
        state.level = 0.0,
        state.mana = 0.0,
        state.direction = 0,
        std::memset(state.effects, 0, sizeof(state.effects));
        // )

        msg.players.push_back(state);
    }

    return msg;
}

net::BUILDINGS_UPDATES LobbyContext::getBuildingsUpdates() {
    net::BUILDINGS_UPDATES msg;

    auto& registry = const_cast<ECS::Registry&>(lobby.getRegistry());
    auto& healths = registry.getComponents<addon::eSpec::Health>();

    for (auto&& [entity, health] :
        ECS::IndexedDenseZipper(healths)) {
        if (entity < eField::BUILDINGS_BEGIN || entity > eField::BUILDINGS_END)
            continue;

        net::BuildingsUpdate state {
            .id = static_cast<uint32_t>(entity),
            .hp = static_cast<double>(health.amount)
        };

        msg.buildings.push_back(state);
    }

    return msg;
}

// net::BUILDING_UPDATES LobbyContext::getProjectilesUpdates() {
//     net::BUILDING_UPDATES msg;

//     auto& registry = const_cast<ECS::Registry&>(lobby.getRegistry());
//     auto& healths = registry.getComponents<addon::eSpec::Health>();

//     for (auto&& [entity, health] :
//         ECS::IndexedDenseZipper(healths)) {
//         if (entity < eField::BUILDINGS_BEGIN ||
//            entity > eField::BUILDINGS_END)
//             continue;

//         net::BuildingUpdate state {
//             .id = entity,
//             .hp = static_cast<double>(health.amount)
//         };

//         msg.buildings.push_back(state);
//     }

//     return msg;
// }

std::optional<net::PLAYERS_UPDATES> LobbyContext::tryGetPlayerUpdates() {
    if (!isInGame() || !players_update.checkDelay()) {
        return std::nullopt;
    }

    return getPlayerUpdates();
}

// std::optional<net::PLAYERS_UPDATES> LobbyContext::tryGetBuildingsUpdates() {
//     if (!buildings_updates.checkDelay()) {
//         return std::nullopt;
//     }

//     return getBuildingsUpdates();
// }
