file(GLOB srcs "${CMAKE_CURRENT_SOURCE_DIR}/src/*.cpp")
add_executable(autotester ${srcs})
target_include_directories(autotester PUBLIC include)
target_link_libraries(autotester spa ${AUTOTESTER_LIBRARIES})

if(NOT WIN32)
    target_link_libraries(autotester pthread)
    target_link_options(autotester PRIVATE -no-pie)
endif()
