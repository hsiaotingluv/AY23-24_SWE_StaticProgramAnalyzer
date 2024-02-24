file(GLOB_RECURSE srcs "${CMAKE_CURRENT_SOURCE_DIR}/src/*.cpp" "${CMAKE_CURRENT_SOURCE_DIR}/src/**/*.cpp")
add_executable(unit_testing ${srcs})
target_include_directories(unit_testing PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/include)

target_link_libraries(unit_testing spa)

if(CMAKE_BUILD_TYPE STREQUAL "Debug" AND NOT WIN32)
    message(STATUS "Building unit_testing with AddressSanitizer")
    target_compile_options(unit_testing PRIVATE -fsanitize=address -fsanitize=undefined)
    target_link_options(unit_testing PRIVATE -fsanitize=address -fsanitize=undefined)
endif()

if(TEST_WITH_BENCHMARK)
    target_compile_definitions(unit_testing PRIVATE ENABLE_BENCHMARK)
endif()