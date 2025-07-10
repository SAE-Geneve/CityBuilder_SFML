# Packages : external libraries managed by
find_package(SFML COMPONENTS Graphics Window System CONFIG REQUIRED)

file(GLOB_RECURSE SRC_FILES src/*.cc)
file(GLOB_RECURSE HEADER_FILES include/*.h)

add_library(api ${SRC_FILES} ${HEADER_FILES})

target_include_directories(api PUBLIC include/)
target_link_libraries(api PUBLIC core SFML::System SFML::Graphics SFML::Window)

if(ENABLE_TESTING)
    file(GLOB_RECURSE TEST_FILES test/*.cc)
    add_executable(api_test ${TEST_FILES})
    target_link_libraries(api_test PRIVATE GTest::gtest GTest::gtest_main api)
endif(ENABLE_TESTING)