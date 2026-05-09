# Project-wide compiler warning policy.
#
# Defines an INTERFACE library `project_warnings` that carries the warning
# flags (and -Werror / /WX) for first-party targets. PRIVATE-link it into
# core, api, game, the executable, and the *_test targets. Do not link it
# into externals or vendored interface libraries.
#
# Warning set follows the cppbestpractices / Jason Turner list.

add_library(project_warnings INTERFACE)

set(_project_warnings_msvc
        /W4
        /permissive-
        /WX
        /w14242 /w14254 /w14263 /w14265 /w14287 /we4289 /w14296
        /w14311 /w14545 /w14546 /w14547 /w14549 /w14555 /w14619
        /w14640 /w14826 /w14905 /w14906 /w14928
)

set(_project_warnings_clang
        -Wall
        -Wextra
        -Wshadow
        -Wnon-virtual-dtor
        -Wold-style-cast
        -Wcast-align
        -Wunused
        -Woverloaded-virtual
        -Wpedantic
        -Wconversion
        -Wsign-conversion
        -Wnull-dereference
        -Wdouble-promotion
        -Wformat=2
        -Wimplicit-fallthrough
        -Werror
)

set(_project_warnings_gcc
        ${_project_warnings_clang}
        -Wmisleading-indentation
        -Wduplicated-cond
        -Wduplicated-branches
        -Wlogical-op
        -Wuseless-cast
)

target_compile_options(project_warnings INTERFACE
        $<$<CXX_COMPILER_ID:MSVC>:${_project_warnings_msvc}>
        $<$<CXX_COMPILER_ID:Clang,AppleClang>:${_project_warnings_clang}>
        $<$<CXX_COMPILER_ID:GNU>:${_project_warnings_gcc}>
)
