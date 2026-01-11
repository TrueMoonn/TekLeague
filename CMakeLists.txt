cmake_minimum_required(VERSION 3.10)
project(tek_league)

########## SETUP ##########
set(CMAKE_CXX_STANDARD 23)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${PROJECT_SOURCE_DIR})
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -O3")
set(GAME_SRC_DIR "${PROJECT_SOURCE_DIR}/src")
set(GAME_HDR_DIR "${PROJECT_SOURCE_DIR}/include")
set(TE_PLUGIN_PATH "${PROJECT_SOURCE_DIR}/TrueEngine/plugins")

########## TRUE ENGINE ##########
add_subdirectory(TrueEngine)

########## BINARIES ##########
add_subdirectory(client)
add_subdirectory(server)
