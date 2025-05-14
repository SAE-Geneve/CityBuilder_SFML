cmake_minimum_required(VERSION 3.30)
project(CityBuilder)

set(CMAKE_CXX_STANDARD 20)

# Packages : external libraries managed by
find_package(SFML COMPONENTS Graphics Window System REQUIRED)

add_subdirectory(core/)


file(COPY ${CMAKE_SOURCE_DIR}/assets DESTINATION ${CMAKE_BINARY_DIR})

add_executable(city_builder_game main.cc)

target_link_libraries(city_builder_game PRIVATE core SFML::System SFML::Graphics SFML::Window)

