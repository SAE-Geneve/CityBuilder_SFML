file(GLOB_RECURSE SRC_FILES src/*.cc)
file(GLOB_RECURSE HEADER_FILES include/*.h)

# core was header-only (INTERFACE) until the behaviour-tree library added
# compiled .cc files. It is now a regular (static) compiled library.
add_library(core ${SRC_FILES} ${HEADER_FILES})

target_include_directories(core PUBLIC include/)
