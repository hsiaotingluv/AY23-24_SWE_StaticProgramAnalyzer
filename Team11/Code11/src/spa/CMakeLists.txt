file(GLOB_RECURSE srcs "${CMAKE_CURRENT_SOURCE_DIR}/src/**/*.cpp")
add_library(spa ${srcs})
target_include_directories(spa PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include)

if(CMAKE_BUILD_TYPE STREQUAL "Debug")
    message(STATUS "Building SPA with AddressSanitizer")
    target_compile_options(spa PRIVATE -fsanitize=address)
    target_link_options(spa PRIVATE -fsanitize=address)
endif()
