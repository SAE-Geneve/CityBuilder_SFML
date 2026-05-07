add_library(FastNoiseLit INTERFACE)
target_include_directories(FastNoiseLit INTERFACE
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include/>
)