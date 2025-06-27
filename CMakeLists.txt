cmake_minimum_required(VERSION 3.28)

project(Project3)

set(CMAKE_CXX_STANDARD 11)
add_executable(Project3 main.cpp
        grid.h
        grid.cpp
        leaderboard.h
        leaderboard.cpp)

set(CMAKE_CXX_COMPILER /usr/bin/clang++)
set(SFML_DIR /opt/homebrew/Cellar/sfml/2.6.1/ilb/cmake/SFML)

find_package(SFML 2.6.1 COMPONENTS system window graphics network audio REQUIRED)

include_directories(/opt/homebrew/Cellar/sfml/2.6.1/lib/cmake/SFML)

target_link_libraries(Project3 sfml-system sfml-window sfml-graphics sfml-audio)