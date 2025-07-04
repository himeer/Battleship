cmake_minimum_required(VERSION 3.28)
project(BattleshipProject LANGUAGES CXX C)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

include(FetchContent)

FetchContent_Declare(
    glfw
    GIT_REPOSITORY https://github.com/glfw/glfw.git
    GIT_TAG 3.4
    GIT_SHALLOW ON
    EXCLUDE_FROM_ALL
    SYSTEM
)

FetchContent_Declare(
    stb
    GIT_REPOSITORY https://github.com/nothings/stb.git
    GIT_TAG master
    GIT_SHALLOW ON
    EXCLUDE_FROM_ALL
    SYSTEM
)

set(GLFW_BUILD_EXAMPLES OFF CACHE INTERNAL "")
set(GLFW_BUILD_TESTS OFF CACHE INTERNAL "")
set(GLFW_BUILD_DOCS OFF CACHE INTERNAL "")
set(GLFW_INSTALL OFF CACHE INTERNAL "")
set(GLFW_BUILD_WAYLAND OFF CACHE INTERNAL "")

FetchContent_MakeAvailable(glfw stb)

find_package(OpenGL REQUIRED)


add_library(glad STATIC glad/src/glad.c)
target_include_directories(glad PUBLIC glad/include)

add_executable(Battleship
    src/NTL/Utils/Color.cpp
    src/NTL/System/Window.cpp
    src/NTL/Graphics/Shape.cpp
    src/NTL/Graphics/Shader.cpp
    src/NTL/Graphics/Texture.cpp

    src/Core.cpp
    src/TextureManager.cpp
    src/ShipPlacementMenu.cpp
    src/GameApp.cpp
    src/Ship.cpp
    src/GameGrid.cpp
)
target_compile_features(Battleship PRIVATE cxx_std_20)
target_link_libraries(Battleship PRIVATE OpenGL::GL glfw glad)
target_include_directories(Battleship PUBLIC ${stb_SOURCE_DIR})