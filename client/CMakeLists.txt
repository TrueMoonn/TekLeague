project(tek_league_client)

########## SETUP ##########
set(GAME_CLIENT_SRC_DIR "${PROJECT_SOURCE_DIR}/src")
set(GAME_CLIENT_HDR_DIR "${PROJECT_SOURCE_DIR}/include")

########## GAME ##########
add_executable(${PROJECT_NAME}
    ${GAME_CLIENT_SRC_DIR}/main.cpp
    ${GAME_CLIENT_SRC_DIR}/Client.cpp
    ${GAME_CLIENT_SRC_DIR}/ClientHandlers.cpp
    ${GAME_CLIENT_SRC_DIR}/ClientSenders.cpp

    # SYSTEMS
    ${GAME_CLIENT_SRC_DIR}/systems/update_button.cpp
    ${GAME_CLIENT_SRC_DIR}/systems/minions_animation.cpp
    ${GAME_CLIENT_SRC_DIR}/systems/track_stat.cpp
    ${GAME_CLIENT_SRC_DIR}/systems/updateSticky.cpp

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

# On Windows, use console to see debug logs (same as server)
if(WIN32)
    set_target_properties(${PROJECT_NAME} PROPERTIES
        WIN32_EXECUTABLE FALSE
    )
endif()

########## GET PLUGINS ##########
add_dependencies(${PROJECT_NAME} addon::physic)
add_dependencies(${PROJECT_NAME} addon::entity_spec)
add_dependencies(${PROJECT_NAME} addon::interaction)
add_dependencies(${PROJECT_NAME} addon::sfml)
add_dependencies(${PROJECT_NAME} addon::display)

if(WIN32)
    set(PLUGIN_EXT ".dll")
else()
    set(PLUGIN_EXT ".so")
endif()

add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E make_directory ${CMAKE_CURRENT_SOURCE_DIR}/plugins
    COMMAND ${CMAKE_COMMAND} -E copy_if_different
        $<TARGET_FILE:addon::physic>
        ${CMAKE_CURRENT_SOURCE_DIR}/plugins/physic${PLUGIN_EXT}
    COMMAND ${CMAKE_COMMAND} -E copy_if_different
        $<TARGET_FILE:addon::entity_spec>
        ${CMAKE_CURRENT_SOURCE_DIR}/plugins/entity_spec${PLUGIN_EXT}
    COMMAND ${CMAKE_COMMAND} -E copy_if_different
        $<TARGET_FILE:addon::interaction>
        ${CMAKE_CURRENT_SOURCE_DIR}/plugins/interaction${PLUGIN_EXT}
    COMMAND ${CMAKE_COMMAND} -E copy_if_different
        $<TARGET_FILE:addon::sfml>
        ${CMAKE_CURRENT_SOURCE_DIR}/plugins/sfml${PLUGIN_EXT}
    COMMAND ${CMAKE_COMMAND} -E copy_if_different
        $<TARGET_FILE:addon::display>
        ${CMAKE_CURRENT_SOURCE_DIR}/plugins/display${PLUGIN_EXT}
)
