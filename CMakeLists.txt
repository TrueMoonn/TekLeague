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

########## GAME ##########
add_executable(${PROJECT_NAME}
    ${GAME_SRC_DIR}/main.cpp

    ${GAME_SRC_DIR}/Game.cpp

    # SCENES
    ${GAME_SRC_DIR}/scenes/InGame.cpp
    ${GAME_SRC_DIR}/scenes/Menu.cpp

    # COMPONENTS
    ${GAME_SRC_DIR}/components/champion.cpp
    ${GAME_SRC_DIR}/components/stat_pool.cpp

    # SYSTEMS
    ${GAME_SRC_DIR}/systems/champ_mov.cpp
)

target_include_directories(${PROJECT_NAME}
    PUBLIC
        ${TE_HDR_DIR}
        ${GAME_HDR_DIR}
)

target_link_libraries(${PROJECT_NAME}
    PRIVATE
        TrueEngine
)

########## GET PLUGINS ##########
add_dependencies(${PROJECT_NAME} addon::physic)
add_dependencies(${PROJECT_NAME} addon::interaction)
add_dependencies(${PROJECT_NAME} addon::entity_spec)
add_dependencies(${PROJECT_NAME} addon::sfml)
add_dependencies(${PROJECT_NAME} addon::display)

add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E make_directory ${CMAKE_CURRENT_SOURCE_DIR}/plugins
    COMMAND ${CMAKE_COMMAND} -E copy_if_different
        ${TE_PLUGIN_PATH}/physic.so
        ${CMAKE_CURRENT_SOURCE_DIR}/plugins/physic.so
    COMMAND ${CMAKE_COMMAND} -E copy_if_different
        ${TE_PLUGIN_PATH}/interaction.so
        ${CMAKE_CURRENT_SOURCE_DIR}/plugins/interaction.so
    COMMAND ${CMAKE_COMMAND} -E copy_if_different
        ${TE_PLUGIN_PATH}/entity_spec.so
        ${CMAKE_CURRENT_SOURCE_DIR}/plugins/entity_spec.so
    COMMAND ${CMAKE_COMMAND} -E copy_if_different
        ${TE_PLUGIN_PATH}/sfml.so
        ${CMAKE_CURRENT_SOURCE_DIR}/plugins/sfml.so
    COMMAND ${CMAKE_COMMAND} -E copy_if_different
        ${TE_PLUGIN_PATH}/display.so
        ${CMAKE_CURRENT_SOURCE_DIR}/plugins/display.so
)
