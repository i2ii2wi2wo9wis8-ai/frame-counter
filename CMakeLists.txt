cmake_minimum_required(VERSION 3.21)
set(CMAKE_CXX_STANDARD 20)

# GEODE_SDK yolunu bul (GitHub Actions bunu arka planda indirip ortam değişkenine ekler)
if(DEFINED ENV{GEODE_SDK})
    set(GEODE_SDK "$ENV{GEODE_SDK}")
else()
    # Eğer yerelde derliyorsan buraya kendi SDK yolunu yazabilirsin
    set(GEODE_SDK "${CMAKE_CURRENT_SOURCE_DIR}/../geode-sdk")
endif()

# Geode'nin kendi CMake dosyasını dahil et
include("${GEODE_SDK}/cmake/Geode.cmake")

project(frame_counter)

add_library(${PROJECT_NAME} SHARED
    src/main.cpp
)

target_link_libraries(${PROJECT_NAME} Geode::Geode)
