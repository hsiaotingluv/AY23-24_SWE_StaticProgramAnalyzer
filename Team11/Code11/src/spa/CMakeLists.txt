file(GLOB_RECURSE srcs "${CMAKE_CURRENT_SOURCE_DIR}/src/**/*.cpp")
add_library(spa ${srcs})
target_include_directories(spa PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include)

if(CMAKE_BUILD_TYPE STREQUAL "Debug" AND NOT WIN32)
    message(STATUS "Building SPA with AddressSanitizer")
    target_compile_options(spa PRIVATE -fsanitize=address -fsanitize=undefined)
    target_link_options(spa PRIVATE -fsanitize=address -fsanitize=undefined)
endif()

message(STATUS "Building SPA in Milestone1 mode")
target_compile_definitions(spa PRIVATE MILESTONE1)

if(CMAKE_BUILD_TYPE STREQUAL "Debug" AND CMAKE_CXX_COMPILER_ID STREQUAL "GNU")
    append_coverage_compiler_flags()

    message(STATUS " Building SPA with coverage ")
    setup_target_for_coverage_lcov(
        NAME ${PROJECT_NAME}_coverage
        EXECUTABLE unit_testing
        DEPENDENCIES unit_testing spa
        EXCLUDE "/usr/include/*")
endif()