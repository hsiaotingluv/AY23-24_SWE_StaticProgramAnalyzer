file(GLOB srcs "${CMAKE_CURRENT_SOURCE_DIR}/src/*/*.cpp")
add_library(spa ${srcs}
        src/PKB/Stores/FollowsStore.cpp
        include/pkb/FollowsStore.h
        include/pkb/PkbTypes.h
        src/PKB/Stores/ParentStore.cpp
        include/pkb/ParentStore.h)
target_include_directories(spa PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include)

if(CMAKE_BUILD_TYPE STREQUAL "Debug")
    message(STATUS "Building SPA with AddressSanitizer")
    target_compile_options(spa PRIVATE -fsanitize=address)
    target_link_options(spa PRIVATE -fsanitize=address)
endif()
