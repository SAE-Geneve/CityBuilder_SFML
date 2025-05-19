# Cmake Versio
cmake_minimum_required(VERSION 3.30)

# Create Solution
project(CityBuilder)

# C++ Version
set(CMAKE_CXX_STANDARD 20)

# Packages : external libraries managed by
find_package(SFML COMPONENTS Graphics Window System REQUIRED)

# Creating Architecture and libraries
add_subdirectory(core/)
add_subdirectory(api/)
add_subdirectory(game/)

# Manage Ressources
# Delete all and copy
file(REMOVE_RECURSE "${CMAKE_BINARY_DIR}/_assets")
file(COPY ${CMAKE_SOURCE_DIR}/_assets DESTINATION ${CMAKE_BINARY_DIR})

# Add executable project
add_executable(city_builder_game main.cc)
# This project is linked to its own libraries
target_link_libraries(city_builder_game PRIVATE game)

target_compile_options(city_builder_game PRIVATE
        $<$<AND:$<CONFIG:Debug>,$<CXX_COMPILER_ID:MSVC>>:/Od /Zi>
        $<$<AND:$<CONFIG:Debug>,$<NOT:$<CXX_COMPILER_ID:MSVC>>>:-g -O0>
)
