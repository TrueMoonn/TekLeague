project(tek_league_server)

########## SETUP ##########
set(GAME_SERVER_SRC_DIR "${PROJECT_SOURCE_DIR}/src")
set(GAME_SERVER_HDR_DIR "${PROJECT_SOURCE_DIR}/include")

########## GAME ##########
add_executable(${PROJECT_NAME}
    ${GAME_SERVER_SRC_DIR}/main.cpp
    ${GAME_SERVER_SRC_DIR}/Server.cpp
    ${GAME_SERVER_SRC_DIR}/ServerHandlers.cpp
    ${GAME_SERVER_SRC_DIR}/ServerSenders.cpp
    ${CMAKE_SOURCE_DIR}/src/Lobby/LobbyContext.cpp
    ${CMAKE_SOURCE_DIR}/src/Lobby/Lobby.cpp
)

target_include_directories(${PROJECT_NAME}
    PUBLIC
        ${TE_HDR_DIR}
        ${GAME_HDR_DIR}
        ${GAME_SERVER_HDR_DIR}
)

target_link_libraries(${PROJECT_NAME}
    PRIVATE
        TekLeague
        TrueEngine
)

########## GET PLUGINS ##########
add_dependencies(${PROJECT_NAME} addon::physic)
add_dependencies(${PROJECT_NAME} addon::interaction)
add_dependencies(${PROJECT_NAME} addon::entity_spec)

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
)
