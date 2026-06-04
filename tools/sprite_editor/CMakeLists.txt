find_package(SFML COMPONENTS Graphics Window System CONFIG REQUIRED)
find_package(ImGui-SFML CONFIG REQUIRED)
find_package(nlohmann_json CONFIG REQUIRED)

file(GLOB SRC_FILES *.cc)
file(GLOB HEADER_FILES *.h)

add_executable(sprite_editor ${SRC_FILES} ${HEADER_FILES})

# HiDPI fix: declare the tool DPI-unaware so the GL framebuffer stays 1:1 with
# sf::Window::getSize(), which ImGui-SFML 3.0 assumes. CMake embeds .manifest
# source files via the MSVC linker. See dpi_unaware.manifest for the rationale.
if(WIN32)
    target_sources(sprite_editor PRIVATE dpi_unaware.manifest)
endif()

target_link_libraries(sprite_editor PRIVATE
        core
        SFML::System SFML::Graphics SFML::Window
        ImGui-SFML::ImGui-SFML
        nlohmann_json::nlohmann_json)
target_link_libraries(sprite_editor PRIVATE project_warnings)
