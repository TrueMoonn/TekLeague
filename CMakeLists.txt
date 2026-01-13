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
add_subdirectory(TrueEngine)

########## COMMUN ##########
add_library(${PROJECT_NAME} STATIC
    ${GAME_SRC_DIR}/Game.cpp

    # COMPONENTS
    ${GAME_SRC_DIR}/components/zone_ranged.cpp
    ${GAME_SRC_DIR}/components/champion.cpp
    ${GAME_SRC_DIR}/components/stat_pool.cpp
    ${GAME_SRC_DIR}/components/button.cpp
    ${GAME_SRC_DIR}/components/inventory.cpp
    ${GAME_SRC_DIR}/components/item.cpp
    ${GAME_SRC_DIR}/components/target.cpp
    ${GAME_SRC_DIR}/components/auto_track.cpp

    # SYSTEMS
    ${GAME_SRC_DIR}/systems/champ_mov.cpp
    ${GAME_SRC_DIR}/systems/entity_direction.cpp
    ${GAME_SRC_DIR}/systems/target_player.cpp
    ${GAME_SRC_DIR}/systems/track_target.cpp

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
