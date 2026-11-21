cmake_minimum_required(VERSION 3.21)
set(CMAKE_CXX_STANDARD 20)

# Geode SDK'yı projeye dahil et
include(${CMAKE_CURRENT_SOURCE_DIR}/../Geode.cmake OPTIONAL)

# Eğer Geode build ortamındaysak (GitHub Actions gibi) ayarları yükle
if(DEFINED ENV{GEODE_SDK})
    include($ENV{GEODE_SDK}/cmake/Geode.cmake)
else()
    find_package(Geode REQUIRED)
endif()

project(frame_counter)

add_library(${PROJECT_NAME} SHARED
    src/main.cpp
)

target_link_libraries(${PROJECT_NAME} Geode::Geode)
