file(GLOB_RECURSE srcs "${CMAKE_CURRENT_SOURCE_DIR}/src/*.cpp")
file(GLOB_RECURSE headers "${CMAKE_CURRENT_SOURCE_DIR}/src/*.h" "${CMAKE_CURRENT_SOURCE_DIR}/src/*.hpp")
add_executable(integration_testing ${srcs})

target_link_libraries(integration_testing spa)

if(CMAKE_BUILD_TYPE STREQUAL "Debug" AND NOT WIN32)
    message(STATUS "Building integration_testing with AddressSanitizer")
    target_compile_options(integration_testing PRIVATE -fsanitize=address -fsanitize=undefined)
    target_link_options(integration_testing PRIVATE -fsanitize=address -fsanitize=undefined)
endif()