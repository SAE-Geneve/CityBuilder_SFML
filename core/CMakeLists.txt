file(GLOB_RECURSE SRC_FILES src/*.cc)
file(GLOB_RECURSE HEADER_FILES include/*.h)

add_library(core ${SRC_FILES} ${HEADER_FILES})

target_include_directories(core PUBLIC include/)
target_link_libraries(core PRIVATE SFML::System SFML::Graphics SFML::Window)
