file(GLOB_RECURSE SRC_FILES src/*.cc)
file(GLOB_RECURSE HEADER_FILES include/*.h)

add_library(game ${SRC_FILES} ${HEADER_FILES})

target_include_directories(game PUBLIC include/)
target_link_libraries(game PUBLIC core api)

if(ENABLE_TESTING)
    file(GLOB_RECURSE TEST_FILES test/*.cc)
    add_executable(game_test ${TEST_FILES})
    target_link_libraries(game_test PRIVATE GTest::gtest GTest::gtest_main game)
endif(ENABLE_TESTING)