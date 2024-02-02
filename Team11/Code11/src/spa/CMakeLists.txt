file(GLOB srcs "${CMAKE_CURRENT_SOURCE_DIR}/src/*/*.cpp")
add_library(spa ${srcs})
target_include_directories(spa PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include)
