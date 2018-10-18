cmake_minimum_required(VERSION 2.8)

# Specify default build type if none provided (before project() command)
# (you can also set it on the command line: -D CMAKE_BUILD_TYPE=Release)
if(NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE Release CACHE STRING "Choose the type of build (Debug or Release)" FORCE)
endif()

# For generators with multiple configurations (like VS), only allow Debug and Release
if(CMAKE_CONFIGURATION_TYPES)
    set(CMAKE_CONFIGURATION_TYPES "Debug;Release" CACHE STRING "Limited configurations" FORCE)
    mark_as_advanced(CMAKE_CONFIGURATION_TYPES)
endif()

# Project name
project(crank)

# Specific compiler options - set C++11 flag for g++ and clang
if(CMAKE_COMPILER_IS_GNUCXX)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")
elseif(CMAKE_CXX_COMPILER_ID STREQUAL "Clang")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")

    # Mac OS X: clang uses libc++ standard library
    if(${CMAKE_SYSTEM_NAME} MATCHES "Darwin")
        set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -stdlib=libc++")
    endif()
endif()

######################

# Add directory containing FindSFML.cmake to module path
set(CMAKE_MODULE_PATH "${CMAKE_SOURCE_DIR}/cmake_modules/;${CMAKE_MODULE_PATH}")

# Find any version 2.X of SFML
# See the FindSFML.cmake file for additional details and instructions
find_package(SFML 2 COMPONENTS system window graphics network audio)

# If found, include and link; otherwise output error message
if(SFML_FOUND)
    include_directories(${SFML_INCLUDE_DIR})
else()
    set(SFML_ROOT "" CACHE_PATH "SFML top-level directory")
    message("\n->SFML directory not found. Set SFML_ROOT to SFML's top-level path (containing \"include\" and \"lib\" directories).")
    message("-> Make sure the SFML libraries with the same configuration (Release/Debug, Static/Dynamic) exist.\n")
endif()

######################

set(SOURCES
    Ship.cpp
    Animation.cpp
    Application.cpp
    Button.cpp
    BloomEffect.cpp
    Command.cpp
    CommandQueue.cpp
    Component.cpp
    Container.cpp
    DataTables.cpp
    EmitterNode.cpp
    Entity.cpp
    GameOverState.cpp
    GameServer.cpp
    GameState.cpp
    KeyBinding.cpp
    Label.cpp
    LoadingState.cpp
    MenuState.cpp
    MultiplayerGameState.cpp
    MusicPlayer.cpp
    NetworkNode.cpp
    PauseState.cpp
    ParallelTask.cpp
    ParticleNode.cpp
    Pickup.cpp
    Player.cpp
    PostEffect.cpp
    Projectile.cpp
    SceneNode.cpp
    SettingsState.cpp
    SpriteNode.cpp
    TextNode.cpp
    SoundNode.cpp
    SoundPlayer.cpp
    State.cpp
    StateStack.cpp
    TitleState.cpp
    Utility.cpp
    World.cpp)

set(PROJECT_DIR "${PROJECT_SOURCE_DIR}/..")
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${PROJECT_DIR}/bin)

include_directories("${PROJECT_DIR}/include")

add_executable(${PROJECT_NAME} main.cpp ${SOURCES})
# set_target_properties(${PROJECT_NAME} PROPERTIES OUTPUT_NAME "${PROJECT_DIR}/bin/${EXECUTABLE_NAME}")

# Link SFML
target_link_libraries(${PROJECT_NAME} ${SFML_LIBRARIES})

# Install target
install(TARGETS ${EXECUTABLE_NAME} DESTINATION bin)

# CPack packaging
# include(InstallRequiredSystemLibraries)
# set(CPACK_RESOURCE_FILE_LICENSE "${CMAKE_SOURCE_DIR}/COPYING")
# set(CPACK_PACKAGE_VERSION_MAJOR "${crank_VERSION_MAJOR}")
# set(CPACK_PACKAGE_VERSION_MINOR "${crank_VERSION_MINOR}")
# include(CPack)
