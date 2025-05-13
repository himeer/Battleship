cmake_minimum_required(VERSION 3.28)
project(BattleshipProject LANGUAGES CXX)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

include(FetchContent)
# FetchContent_Declare(SFML
#     GIT_REPOSITORY https://github.com/SFML/SFML.git
#     GIT_TAG 3.0.1
#     GIT_SHALLOW ON
#     EXCLUDE_FROM_ALL
#     SYSTEM)
# FetchContent_MakeAvailable(SFML)
FetchContent_Declare(
    glfw
    GIT_REPOSITORY https://github.com/glfw/glfw.git
    GIT_TAG 3.4
    GIT_SHALLOW ON
    EXCLUDE_FROM_ALL
    SYSTEM
)

set(GLFW_BUILD_EXAMPLES OFF CACHE INTERNAL "")
set(GLFW_BUILD_TESTS OFF CACHE INTERNAL "")
set(GLFW_BUILD_DOCS OFF CACHE INTERNAL "")
set(GLFW_INSTALL OFF CACHE INTERNAL "")

FetchContent_MakeAvailable(glfw)

find_package(OpenGL REQUIRED)

add_executable(Battleship
    src/NTL/Utils/Color.cpp
    src/NTL/System/Window.cpp
    src/NTL/Graphics/Shape.cpp

    src/Core.cpp
    # src/ShipPlacementMenu.cpp
)
target_compile_features(Battleship PRIVATE cxx_std_20)
target_link_libraries(Battleship PRIVATE OpenGL::GL glfw)