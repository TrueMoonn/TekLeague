project(tek_league_client)

########## SETUP ##########
set(GAME_CLIENT_SRC_DIR "${PROJECT_SOURCE_DIR}/src")
set(GAME_CLIENT_HDR_DIR "${PROJECT_SOURCE_DIR}/include")

########## GAME ##########
add_executable(${PROJECT_NAME}
    ${GAME_CLIENT_SRC_DIR}/main.cpp
    ${GAME_CLIENT_SRC_DIR}/Client.cpp

    # SYSTEMS
    ${GAME_CLIENT_SRC_DIR}/systems/update_button.cpp

    # SCENES
    ${GAME_CLIENT_SRC_DIR}/scenes/Main.cpp
    ${GAME_CLIENT_SRC_DIR}/scenes/lobby/Lobby.cpp
    ${GAME_CLIENT_SRC_DIR}/scenes/lobby/SearchLobby.cpp
    ${GAME_CLIENT_SRC_DIR}/scenes/lobby/InLobby.cpp
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
        TekLeague
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
