find_package(SFML COMPONENTS Graphics Window System CONFIG REQUIRED)
find_package(ImGui-SFML CONFIG REQUIRED)
find_package(nlohmann_json CONFIG REQUIRED)

file(GLOB SRC_FILES *.cc)
file(GLOB HEADER_FILES *.h)

add_executable(sprite_editor ${SRC_FILES} ${HEADER_FILES})

target_link_libraries(sprite_editor PRIVATE
        core
        SFML::System SFML::Graphics SFML::Window
        ImGui-SFML::ImGui-SFML
        nlohmann_json::nlohmann_json)
target_link_libraries(sprite_editor PRIVATE project_warnings)
