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
    ${GAME_SERVER_SRC_DIR}/LobbyContext.cpp
    ${GAME_SERVER_SRC_DIR}/LobbyContextGetUpdates.cpp
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

# On Windows, ensure console window is shown for logs
if(WIN32)
    set_target_properties(${PROJECT_NAME} PROPERTIES
        WIN32_EXECUTABLE FALSE
    )
endif()

########## GET PLUGINS ##########
add_dependencies(${PROJECT_NAME} addon::physic)
add_dependencies(${PROJECT_NAME} addon::interaction)
add_dependencies(${PROJECT_NAME} addon::entity_spec)

if(WIN32)
    set(PLUGIN_EXT ".dll")
else()
    set(PLUGIN_EXT ".so")
endif()

add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E make_directory ${CMAKE_CURRENT_SOURCE_DIR}/plugins
    COMMAND ${CMAKE_COMMAND} -E copy_if_different
        ${TE_PLUGIN_PATH}/physic${PLUGIN_EXT}
        ${CMAKE_CURRENT_SOURCE_DIR}/plugins/physic${PLUGIN_EXT}
    COMMAND ${CMAKE_COMMAND} -E copy_if_different
        ${TE_PLUGIN_PATH}/interaction${PLUGIN_EXT}
        ${CMAKE_CURRENT_SOURCE_DIR}/plugins/interaction${PLUGIN_EXT}
    COMMAND ${CMAKE_COMMAND} -E copy_if_different
        ${TE_PLUGIN_PATH}/entity_spec${PLUGIN_EXT}
        ${CMAKE_CURRENT_SOURCE_DIR}/plugins/entity_spec${PLUGIN_EXT}
)
