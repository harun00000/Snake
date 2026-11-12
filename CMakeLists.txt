cmake_minimum_required(VERSION 3.10)

project(SnakeGame)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# to find SFML (else -> error)
find_package(SFML 2.5 COMPONENTS graphics window system audio REQUIRED)

add_executable(snake
    app/main.cpp
    src/snake.cpp
    src/food.cpp
    src/game.cpp
    src/logger.cpp
)

target_include_directories(snake PRIVATE include)

# connect these libraries to program
target_link_libraries(snake
    sfml-graphics
    sfml-window
    sfml-system
    sfml-audio
)