file(GLOB srcs "${CMAKE_CURRENT_SOURCE_DIR}/src/*/*.cpp")
add_library(spa ${srcs}
        src/pkb/Stores/FollowsStore.cpp
        include/pkb/Stores/FollowsStore.h
        include/pkb/CommonTypes.h
        src/pkb/Stores/ParentStore.cpp
        include/pkb/Stores/ParentStore.h
        include/pkb/Facades/ReadFacade.h
        include/pkb/Facades/ReadFacade.h
        include/pkb/Facades/WriteFacade.h
        src/pkb/Facades/ReadFacade.cpp
        src/pkb/Facades/WriteFacade.cpp
        include/pkb/Stores/EntityStore.h
        src/pkb/Stores/EntityStore.cpp)
target_include_directories(spa PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include)

if(CMAKE_BUILD_TYPE STREQUAL "Debug")
    message(STATUS "Building SPA with AddressSanitizer")
    target_compile_options(spa PRIVATE -fsanitize=address)
    target_link_options(spa PRIVATE -fsanitize=address)
endif()
