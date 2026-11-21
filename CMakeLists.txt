cmake_minimum_required(VERSION 3.21)
set(CMAKE_CXX_STANDARD 20)

project(frame_counter)

add_library(${PROJECT_NAME} SHARED
    src/main.cpp
)

target_link_libraries(${PROJECT_NAME} Geode::Geode)
