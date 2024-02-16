add_executable(autotester "src/TestWrapper.cpp")
target_include_directories(autotester PUBLIC include)
target_link_libraries(autotester spa ${AUTOTESTER_LIBRARIES})

if(CMAKE_BUILD_TYPE STREQUAL "Debug" AND NOT WIN32)
    message(STATUS "Building autotester with AddressSanitizer")
    target_compile_options(autotester PRIVATE -fsanitize=address -fsanitize=undefined)
    target_link_options(autotester PRIVATE -fsanitize=address -fsanitize=undefined)

    message(STATUS "Building local runner")
    add_executable(local_runner "src/main.cpp" "src/TestWrapper.cpp")
    target_include_directories(local_runner PUBLIC include)
    target_link_libraries(local_runner spa)

    target_compile_options(local_runner PRIVATE -fsanitize=address -fsanitize=undefined)
    target_link_options(local_runner PRIVATE -fsanitize=address -fsanitize=undefined)
endif()

if(NOT WIN32)
    target_link_libraries(autotester pthread)
    target_link_options(autotester PRIVATE -no-pie)
endif()
