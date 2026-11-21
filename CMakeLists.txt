cmake_minimum_required(VERSION 3.21)
set(CMAKE_CXX_STANDARD 20)

project(frame_counter)

# GitHub Actions'un indirdiği Geode SDK yolunu Cmake'e tanıtıyoruz
if(DEFINED ENV{GEODE_SDK})
    set(CMAKE_MODULE_PATH "$ENV{GEODE_SDK}/cmake")
endif()

find_package(Geode REQUIRED)

add_library(${PROJECT_NAME} SHARED
    src/main.cpp
)

target_link_libraries(${PROJECT_NAME} Geode::Geode)
