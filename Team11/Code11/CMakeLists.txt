cmake_minimum_required(VERSION 3.20.2)
project(SPA)

option(TEST_WITH_BENCHMARK "Enable benchmarks on test suites" OFF)
set(CMAKE_VERBOSE_MAKEFILE on)

set(CMAKE_MODULE_PATH "${CMAKE_MODULE_PATH};${PROJECT_SOURCE_DIR}/cmake")
message(STATUS "${PROJECT_SOURCE_DIR} ${PROJECT_BINARY_DIR}")

if("${PROJECT_SOURCE_DIR}" STREQUAL "${PROJECT_BINARY_DIR}")
    message(FATAL_ERROR "In-source builds are forbidden.")
endif()

if(NOT CMAKE_BUILD_TYPE OR "${CMAKE_BUILD_TYPE}" STREQUAL "Release")
    message(STATUS "Default to Release")
    set(CMAKE_BUILD_TYPE Release)

    if(NOT MSVC)
        set(CMAKE_CXX_FLAGS_DEBUG "-g")
        set(CMAKE_CXX_FLAGS_RELEASE "-O3")
    endif()
elseif("${CMAKE_BUILD_TYPE}" STREQUAL "Debug")
    message(STATUS "Build type: Debug")
    add_definitions(-DDEBUG)
    set(CMAKE_BUILD_TYPE Debug)
else()
    message(STATUS "Build type: ${CMAKE_BUILD_TYPE}")
endif()

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED on)

if(CMAKE_BUILD_TYPE STREQUAL "Debug")
    message(STATUS "Export compile_commands.json")
    set(CMAKE_EXPORT_COMPILE_COMMANDS 1)
endif()

set(AUTOTESTER_ROOT "${CMAKE_CURRENT_LIST_DIR}/lib/autotester/${CMAKE_SYSTEM_PROCESSOR}/${CMAKE_SYSTEM_NAME}/${CMAKE_BUILD_TYPE}")
include(${CMAKE_MODULE_PATH}/CodeCoverage.cmake)

list(APPEND CMAKE_PREFIX_PATH "${AUTOTESTER_ROOT}")
find_package(autotester REQUIRED)
include_directories("${CMAKE_CURRENT_LIST_DIR}/lib") # include catch.hpp

add_subdirectory(src/spa)
add_subdirectory(src/autotester)

# add_subdirectory(src/autotester_gui)
add_subdirectory(src/unit_testing)
add_subdirectory(src/integration_testing)
