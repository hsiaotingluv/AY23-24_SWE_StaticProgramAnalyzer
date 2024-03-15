file(GLOB_RECURSE srcs "${CMAKE_CURRENT_SOURCE_DIR}/src/*.cpp" "${CMAKE_CURRENT_SOURCE_DIR}/src/**/*.cpp")
add_executable(integration_testing ${srcs})

target_link_libraries(integration_testing spa)

if(CMAKE_BUILD_TYPE STREQUAL "Debug" AND NOT WIN32)
    message(STATUS "Building integration_testing with UBSan")
    target_compile_options(integration_testing PRIVATE -fsanitize=undefined)
    target_link_options(integration_testing PRIVATE -fsanitize=undefined)

    if(ADDRESS_SANITIZER)
        message(STATUS "Building integration_testing with ASan")
        target_compile_options(integration_testing PRIVATE -fsanitize=address)
        target_link_options(integration_testing PRIVATE -fsanitize=address)
    endif()
endif()