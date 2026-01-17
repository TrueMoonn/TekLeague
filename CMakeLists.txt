cmake_minimum_required(VERSION 3.10)
project(TekLeague)

########## SETUP ##########
set(CMAKE_CXX_STANDARD 23)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${PROJECT_SOURCE_DIR})
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -O3")
set(GAME_SRC_DIR "${PROJECT_SOURCE_DIR}/src")
set(GAME_HDR_DIR "${PROJECT_SOURCE_DIR}/include")
set(TE_PLUGIN_PATH "${PROJECT_SOURCE_DIR}/TrueEngine/plugins")

########## PROTOCOL GENERATION ##########
message(STATUS "Generating protocol files...")
execute_process(
    COMMAND python3 ${PROJECT_SOURCE_DIR}/TrueEngine/Network/tools/generate_protocol.py ${PROJECT_SOURCE_DIR}/config/protocol.json
    WORKING_DIRECTORY ${PROJECT_SOURCE_DIR}/TrueEngine/Network
    RESULT_VARIABLE PROTOCOL_RESULT
)

if(NOT PROTOCOL_RESULT EQUAL 0)
    message(WARNING "Protocol generation failed, but continuing build...")
else()
    message(STATUS "Protocol files generated successfully!")
endif()

########## TRUE ENGINE ##########
set(USE_CUSTOM_PROTOCOL ON CACHE BOOL "Use TekLeague protocol" FORCE)
add_subdirectory(TrueEngine)

########## COMMUN ##########
add_library(${PROJECT_NAME} STATIC
    ${GAME_SRC_DIR}/Game.cpp

    # COMPONENTS
    ${GAME_SRC_DIR}/components/champion.cpp

    ${GAME_SRC_DIR}/components/competences/target.cpp
    ${GAME_SRC_DIR}/components/competences/auto_attack.cpp
    ${GAME_SRC_DIR}/components/competences/auto_track.cpp
    ${GAME_SRC_DIR}/components/competences/spells.cpp

    ${GAME_SRC_DIR}/components/items/inventory.cpp
    ${GAME_SRC_DIR}/components/items/item.cpp

    ${GAME_SRC_DIR}/components/stats/stat_pool.cpp
    ${GAME_SRC_DIR}/components/stats/xp.cpp
    ${GAME_SRC_DIR}/components/stats/mana.cpp
    ${GAME_SRC_DIR}/components/stats/health.cpp
    ${GAME_SRC_DIR}/components/stats/gold.cpp

    ${GAME_SRC_DIR}/components/ui/button.cpp
    ${GAME_SRC_DIR}/components/ui/game/sticky.cpp
    ${GAME_SRC_DIR}/components/ui/game/track_stat.cpp
    ${GAME_SRC_DIR}/components/stats/stat_pool.cpp
    ${GAME_SRC_DIR}/components/items/inventory.cpp
    ${GAME_SRC_DIR}/components/items/item.cpp
    ${GAME_SRC_DIR}/components/competences/target.cpp
    ${GAME_SRC_DIR}/components/competences/auto_track.cpp
    ${GAME_SRC_DIR}/components/minions/minions.cpp
    ${GAME_SRC_DIR}/components/minions/minionsRoute.cpp

    # SYSTEMS
    ${GAME_SRC_DIR}/systems/champ_mov.cpp
    ${GAME_SRC_DIR}/systems/entity_direction.cpp
    ${GAME_SRC_DIR}/systems/auto_attacks.cpp
    ${GAME_SRC_DIR}/systems/track_target.cpp
    ${GAME_SRC_DIR}/systems/deal_damage.cpp
    ${GAME_SRC_DIR}/systems/kill_all_entity.cpp
    ${GAME_SRC_DIR}/systems/TrackStat.cpp
    ${GAME_SRC_DIR}/systems/updateSticky.cpp
    ${GAME_SRC_DIR}/systems/minions_mov.cpp

)

target_include_directories(${PROJECT_NAME}
    PUBLIC
        ${GAME_HDR_DIR}
        ${TE_HDR_DIR}
)

target_link_libraries(${PROJECT_NAME}
    PRIVATE
        TrueEngine
)

########## BINARIES ##########
add_subdirectory(client)
add_subdirectory(server)
