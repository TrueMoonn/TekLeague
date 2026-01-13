project(tek_league_client)

########## SETUP ##########
set(GAME_CLIENT_SRC_DIR "${PROJECT_SOURCE_DIR}/src")
set(GAME_CLIENT_HDR_DIR "${PROJECT_SOURCE_DIR}/include")

########## GAME ##########
add_executable(${PROJECT_NAME}
    ########## GLOBAL ##########
    ${GAME_SRC_DIR}/Game.cpp

    # COMPONENTS
    ${GAME_SRC_DIR}/components/champion.cpp
    ${GAME_SRC_DIR}/components/stat_pool.cpp
    ${GAME_SRC_DIR}/components/directed.cpp
    ${GAME_SRC_DIR}/components/button.cpp

    # SYSTEMS
    ${GAME_SRC_DIR}/systems/champ_mov.cpp
    ${GAME_SRC_DIR}/systems/entity_direction.cpp

    ########## LOCAL ##########
    ${GAME_CLIENT_SRC_DIR}/main.cpp
    ${GAME_CLIENT_SRC_DIR}/Client.cpp

    # SYSTEMS
    ${GAME_CLIENT_SRC_DIR}/systems/update_button.cpp

    # SCENES
    ${GAME_CLIENT_SRC_DIR}/scenes/Main.cpp
    ${GAME_CLIENT_SRC_DIR}/scenes/SearchLobby.cpp
    ${GAME_CLIENT_SRC_DIR}/scenes/Parameters.cpp
    ${GAME_CLIENT_SRC_DIR}/scenes/InGame.cpp
)

target_include_directories(${PROJECT_NAME}
    PUBLIC
        ${TE_HDR_DIR}
        ${GAME_HDR_DIR}
        ${GAME_CLIENT_HDR_DIR}
)

target_link_libraries(${PROJECT_NAME}
    PRIVATE
        TrueEngine
)

########## GET PLUGINS ##########
add_dependencies(${PROJECT_NAME} addon::physic)
add_dependencies(${PROJECT_NAME} addon::interaction)
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
        ${TE_PLUGIN_PATH}/sfml.so
        ${CMAKE_CURRENT_SOURCE_DIR}/plugins/sfml.so
    COMMAND ${CMAKE_COMMAND} -E copy_if_different
        ${TE_PLUGIN_PATH}/display.so
        ${CMAKE_CURRENT_SOURCE_DIR}/plugins/display.so
)
