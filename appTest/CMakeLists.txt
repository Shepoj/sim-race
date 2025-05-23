cmake_minimum_required(VERSION 3.21)
project(apptest)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Search for dependencies
find_package(SFML COMPONENTS graphics window system network REQUIRED)
find_package(libphysics REQUIRED)

# Application to configure
set(SRC_DIR ${CMAKE_CURRENT_SOURCE_DIR}/src)

add_executable(${PROJECT_NAME}
    ${SRC_DIR}/main.cpp
    ${SRC_DIR}/Game.cpp
    ${SRC_DIR}/Boat.cpp
    ${SRC_DIR}/Menu.cpp
    ${SRC_DIR}/Obstacle.cpp
    ${SRC_DIR}/Fish.cpp
    ${SRC_DIR}/Port.cpp
)

target_include_directories(${PROJECT_NAME} PRIVATE
    ${SRC_DIR}
    ${CMAKE_CURRENT_SOURCE_DIR}
    /home/austin/Documents/POO/projet_git/sim-race/libphysics/include
)

target_link_libraries(${PROJECT_NAME}
    PRIVATE
    sfml-graphics
    sfml-window
    sfml-system
    sfml-network
    libphysics::libphysics
)

if (WIN32)
    target_link_libraries(${PROJECT_NAME}
        PRIVATE
        sfml-main
    )
endif ()

target_compile_definitions(${PROJECT_NAME} PRIVATE NOMINMAX)

#=========================================
# INSTALL

# Install targets and dependencies according to the configuration
if(NOT WIN32)
    install(
        TARGETS ${PROJECT_NAME}
        RUNTIME_DEPENDENCIES
        DIRECTORIES "$<TARGET_FILE_DIR:sfml-graphics>" "$<TARGET_FILE_DIR:libphysics::libphysics>"
        RUNTIME DESTINATION "$<CONFIG>/bin"
        LIBRARY DESTINATION "$<CONFIG>/lib"
    )
else() 
    install(
        TARGETS ${PROJECT_NAME}
        RUNTIME DESTINATION "$<CONFIG>/bin"
    )
    # Copy DLL
    add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy -t "${CMAKE_INSTALL_PREFIX}/$<CONFIG>/lib" $<TARGET_RUNTIME_DLLS:${PROJECT_NAME}>
        COMMAND_EXPAND_LISTS
    )
endif()

# Install running script
install(PROGRAMS
    ${CMAKE_CURRENT_SOURCE_DIR}/run.sh
    DESTINATION "$<CONFIG>")
