#include <ostream>
#include <unordered_map>
#include <string>
#include <print>
#include <vector>

#include <Network/generated_messages.hpp>
#include <Network/Address.hpp>
#include <ECS/DenseZipper.hpp>
#include <physic/components/position.hpp>
#include <entity_spec/components/health.hpp>
#include <interaction/components/player.hpp>
#include <vector>

#include "LobbyContext.hpp"
#include "ECS/Entity.hpp"
#include "clock.hpp"
#include "components/competences/target.hpp"
#include "components/circle_hitbox.hpp"
#include "components/minions/minions.hpp"
#include "entities.hpp"
#include "systems.hpp"
#include "entity_spec/components/team.hpp"
#include "components/building.hpp"
#include "my.hpp"

LobbyContext::LobbyContext(uint32_t max_players, const std::string& code)
    : lobby(max_players, code, PLUGINS_PATH), max_clients(max_players) {
    for (auto& sys : SERVER_SYSTEMS)
        lobby.createSystem(sys);
}

void LobbyContext::run() {
    try {
        lobby.run();
        checkNash();
    } catch (const std::exception& e) {
        std::println(stderr, "[LobbyContext::run] ERROR: {}", e.what());
    } catch (...) {
        std::println(stderr, "[LobbyContext::run] ERROR: unknown exception");
    }
}

void LobbyContext::checkNash() {
    if (getGameState() == LobbyGameState::PRE_GAME) {
        spawn_nash_delay.restart();
        stay_nash_delay.restart();
        return;
    }
    if (_nashAlive) {
        if (stay_nash_delay.checkDelay()) {
            auto& game = getLobby();
            game.AddKillEntity(_nashE);

            _nashAlive = false;
            _nashE = 0;
            spawn_nash_delay.restart();
        }
    } else {
        if (spawn_nash_delay.checkDelay()) {
            spawnNash();
            _nashAlive = true;
            stay_nash_delay.restart();
        }
    }
}

void LobbyContext::spawnNash() {
    auto& game = getLobby();

    ECS::Entity e = game.nextEntity(eType::CREATURES);

    _nashE = e;
    game.createEntity(e, "nash", {8192 / 2, 400});
    game.entities_queue.emplace_back(e, "nash");
    lobby.run();
}

void LobbyContext::SpawnCreeps() {
    if (!this->spawn_creap.checkDelay(false)) {
        return;
    }

    static te::Timestamp first_creep(0.f);
    static te::Timestamp second_creep((float)0.5);
    static te::Timestamp third_creep(1.f);
    static te::Timestamp fourth_creep((float)1.5);
    static std::vector<int> alr_passed_1;
    static std::vector<int> alr_passed_2;
    static std::vector<int> alr_passed_3;
    static std::vector<int> alr_passed_4;
    static bool is_passed = false;
    if (is_passed) {
        first_creep.toggle();
        second_creep.toggle();
        third_creep.toggle();
        fourth_creep.toggle();
        alr_passed_1.clear();
        alr_passed_2.clear();
        alr_passed_3.clear();
        alr_passed_4.clear();
        is_passed = false;
    }

    for (size_t i = 0; i < CREATURES.size(); ++i) {
        const auto& [mob_name, mob_pos] = CREATURES[i];
        auto& game = getLobby();
        if (first_creep.checkDelay() && std::find(alr_passed_1.begin(), alr_passed_1.end(), i) == alr_passed_1.end()) {
            ECS::Entity e = game.nextEntity(eType::CREATURES);
            game.createEntity(e, mob_name, {mob_pos.x, mob_pos.y});
            //std::cout << "mob spawn 1\n";
            game.entities_queue.emplace_back(e, mob_name);
            alr_passed_1.push_back(i);
            //std::cout << "mob spawn 1\n";
        }
        if (second_creep.checkDelay() && std::find(alr_passed_2.begin(), alr_passed_2.end(), i) == alr_passed_2.end()) {
            ECS::Entity e = game.nextEntity(eType::CREATURES);

            game.createEntity(e, mob_name, {mob_pos.x, mob_pos.y});
            //std::cout << "mob spawn 2\n";
            game.entities_queue.emplace_back(e, mob_name);
            alr_passed_2.push_back(i);
            //std::cout << "mob spawn 2\n";
        }
        if (third_creep.checkDelay() && std::find(alr_passed_3.begin(), alr_passed_3.end(), i) == alr_passed_3.end()) {
            ECS::Entity e = game.nextEntity(eType::CREATURES);

            game.createEntity(e, mob_name, {mob_pos.x, mob_pos.y});
            //std::cout << "mob spawn 3\n";
            game.entities_queue.emplace_back(e, mob_name);
            alr_passed_3.push_back(i);
            //std::cout << "mob spawn 3\n";
        }
        if (fourth_creep.checkDelay() && std::find(alr_passed_4.begin(), alr_passed_4.end(), i) == alr_passed_4.end()) {
            ECS::Entity e = game.nextEntity(eType::CREATURES);

            game.createEntity(e, mob_name, {mob_pos.x, mob_pos.y});
            //std::cout << "mob spawn 4\n";
            game.entities_queue.emplace_back(e, mob_name);
            alr_passed_4.push_back(i);
            //std::cout << "mob spawn 4\n";
            first_creep.toggle();
            second_creep.toggle();
            third_creep.toggle();
            fourth_creep.toggle();
        }
        if (alr_passed_4.size() == 4) {
            is_passed = true;
        }
    }
    if (is_passed)
        this->spawn_creap.restart();
}

const std::string& LobbyContext::getCode() {
    return lobby.getCode();
}

Game& LobbyContext::getLobby() {
    return lobby;
}

bool LobbyContext::addClient(const net::Address& address, uint32_t client_id) {
    if (connected_players.size() >= max_clients)
        return false;
    connected_players[client_id] = address;
    return true;
}

void LobbyContext::removeClient(uint32_t client_id) {
    connected_players.erase(client_id);
}

const std::unordered_map<uint32_t, net::Address>& LobbyContext::getClients(
) const {
    return connected_players;
}

bool LobbyContext::isFull() const {
    return connected_players.size() >= max_clients;
}

void LobbyContext::createOtherEntities() {
    auto& game = getLobby();

    for (size_t i = 0; i < BUILDINGS.size(); ++i) {
        const auto& [tower_name, tower_pos] = BUILDINGS[i];
        ECS::Entity e = game.nextEntity(eType::BUILDINGS);
        game.createEntity(e, tower_name, {tower_pos.x, tower_pos.y});
    }
}

void LobbyContext::createPlayersEntities() {
    auto& game = getLobby();
    auto& players = game.getPlayers();

    auto& teams = game.getComponent<addon::eSpec::Team>();
    auto& positions = game.getComponent<addon::physic::Position2>();
    auto& targets = game.getComponent<Target>();

    for (auto& player : players) {
        ECS::Entity e = game.nextEntity(eType::CHAMPION);
        game.createEntity(e, CHAMPIONS[player.champion]);

        _player_entities[player.id] = e;

        std::size_t team_idx =
            (player.team < TEAMS.size()) ? player.team : 2;
        teams.getComponent(e).name = TEAMS[team_idx];

        if (team_idx == 1) {  // blue
            positions.getComponent(e).x = BLUE_POS_X;
            positions.getComponent(e).y = BLUE_POS_Y;
            targets.getComponent(e).x = BLUE_POS_X;
            targets.getComponent(e).y = BLUE_POS_Y;
        } else {  // red
            positions.getComponent(e).x = RED_POS_X;
            positions.getComponent(e).y = RED_POS_Y;
            targets.getComponent(e).x = RED_POS_X;
            targets.getComponent(e).y = RED_POS_Y;
        }
    }
}

static void setupCollisionBox(Game &game, ECS::Entity id,
    const std::tuple<float, float, float> &coordinates, bool leftSide)
{
    auto &position = game.getComponent<addon::physic::Position2>();
    auto &circle = game.getComponent<CircleHitbox>();
    auto &&[x, y, r] = coordinates;

    if (position.hasComponent(id) && circle.hasComponent(id)) {
        auto &pos = position.getComponent(id);
        auto &c = circle.getComponent(id);

        pos.x = leftSide ? ((x * 2) + r) : (8192 - (x * 2) - r);
        pos.y = ((y * 2) + r);
        c.radius = r;
    }

}

void LobbyContext::createCollisionBoxes() {
    auto& game = getLobby();
    std::array<std::tuple<float, float, float>, 101> coordinates = {{
        {400, 750, 36}, {420, 753, 38}, {442, 751, 45}, {470, 751, 57},
        {486, 768, 60}, {500, 789, 68}, {517, 815, 65}, {522, 840, 75},
        {522, 879, 75}, {521, 892, 78}, {509, 920, 83}, {496, 940, 88},
        {485, 952, 90}, {465, 994, 66}, {453, 1006, 58}, {419, 1027, 45},
        {396, 1041, 40}, {379, 1052, 35}, //BOX0

        {861, 527, 152}, {871, 487, 130}, {872, 601, 200}, {997, 660, 198},
        {1046, 739, 196}, {1057, 811, 202}, {1051, 895, 202}, {1039, 935, 208},
        {972, 1018, 208}, {875, 1087, 204}, {892, 1079, 207}, {798, 1210, 190},
        {825, 1106, 221}, {828, 1292, 138}, // BOX1

        {1273, 1157, 90}, {1258, 1205, 106}, {1231, 1268, 125},
        {1211, 1289, 140}, {1148, 1334, 133}, {1097, 1344, 124},
        {1065, 1369, 101}, {1057, 1426, 45}, //BOX2

        {1092, 480, 80}, {1131, 484, 98}, {1185, 498, 75}, //BOX3

        {1312, 598, 165}, {1449, 762, 134}, {1457, 710, 180}, {1542, 693, 147},
        {1455, 656, 195}, {1431, 636, 147}, {1395, 622, 133}, //BOX4

        {1466, 1206, 105}, {1453, 1230, 175}, {1505, 1246, 165},
        {1566, 1250, 150}, {1646, 1230, 151}, {1759, 1294, 73},
        {1676, 1210, 144}, {1678, 1085, 108}, {1694, 1139, 103}, //BOX5

        {1830, 470, 128}, {1806, 515, 100}, {1841, 484, 158},
        {1900, 498, 158}, {1937, 507, 153}, // BOX6


        {1672, 903, 108}, {1700, 843, 162}, {1729, 800, 200},
        {1807, 803, 191}, {1868, 807, 186}, {1922, 808, 177}, // BOX7

        {1887, 1137, 45}, {1897, 1129, 89}, {1914, 1123, 172},
        {1935, 1195, 144}, {1981, 1266, 100}, // BOX8

        {-1384, -260, 1612}, {-1314, -432, 1606}, {-1448, -837, 1877},
        {-798, -598, 1340}, {-452, -481, 1079}, {-1134, -1660, 2220},
        {-2467, -3882, 4426}, {-1165, -2920, 3250}, {282, -1000, 1258},
        {560, -893, 1148}, {436, -1604, 1889}, {938, -595, 900},
        {1044, -702, 1074}, {1283, -90, 466}, {1346, -94, 521},
        {1515, 122, 320}, {1619, 221, 212}, {1240, -1049, 1314},
        {-3199, -807, 3360}, {-4556, -346, 4946}, {-3491, 909, 4756},
        {-1784, 1331, 3068}, {-1416, 1574, 3452}, {-863, 1662, 3531},
        {-22, 1619, 3164}, {1078, 1552, 1784}, // WORLD BOUNDARIES
    }};

    ECS::Entity id = 0;

    for (auto &co : coordinates) {
        game.createEntity
            (id = game.nextEntity(eType::MAP), "circle_collisions");
        setupCollisionBox(game, id, co, true);

        game.createEntity
            (id = game.nextEntity(eType::MAP), "circle_collisions");
        setupCollisionBox(game, id, co, false);
    }
}

ECS::Entity LobbyContext::getPlayerEntity(uint32_t client_id) const {
    auto it = _player_entities.find(client_id);
    if (it == _player_entities.end())
        return 0;
    return it->second;
}

std::vector<uint8_t> LobbyContext::forceGetData(uint8_t code) {
    switch (code) {
        case static_cast<uint8_t>(net::PLAYERS_UPDATES::ID):
            return getPlayerUpdates().serialize();
        case static_cast<uint8_t>(net::BUILDINGS_UPDATES::ID):
            return getBuildingsUpdates().serialize();
        case static_cast<uint8_t>(net::CREATURES_UPDATES::ID):
            return getCreaturesUpdates().serialize();
        case static_cast<uint8_t>(net::PROJECTILES_UPDATES::ID):
            return getProjectilesUpdates().serialize();
        case static_cast<uint8_t>(net::COLLECTIBLES_UPDATES::ID):
            return getCollectiblesUpdates().serialize();
        case static_cast<uint8_t>(net::INVENTORIES_UPDATES::ID):
            return getInventoriesUpdates().serialize();
        case static_cast<uint8_t>(net::STATS_UPDATES::ID):
            return getStatsUpdates().serialize();
        case static_cast<uint8_t>(net::SCORE::ID):
            return getScore().serialize();
        case static_cast<uint8_t>(net::GAME_DURATION::ID):
            return getGameDuration().serialize();
        case static_cast<uint8_t>(net::SCOREBOARD::ID):
            return getScoreboard().serialize();
        default:
            return {};
    }
}
