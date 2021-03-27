cmake_minimum_required(VERSION 3.17)
project(rgb_matrix)

set(CMAKE_CXX_STANDARD 20)

if (CMAKE_BUILD_TYPE MATCHES Debug)
    add_compile_definitions(DEBUG)
    add_executable(rgb_matrix main.cpp ants/ant.cpp ants/ant.h game.h life/life.cpp life/life.h color.cpp color.h ants/ants.cpp ants/ants.h)
else ()
    include_directories(${PROJECT_SOURCE_DIR}/matrix/lib)
    include_directories(${PROJECT_SOURCE_DIR}/matrix/include)

    file(GLOB_RECURSE RGB_SRC_FILES ${PROJECT_SOURCE_DIR}/matrix/lib/*)
    file(GLOB_RECURSE RGB_HEADER_FILES ${PROJECT_SOURCE_DIR}/matrix/include/*.h)

    add_executable(rgb_matrix ${RGB_SRC_FILES} ${RGB_HEADER_FILES} main.cpp ants/ant.cpp ants/ant.h ants/ants.cpp ants/ants.h game.h life/life.cpp life/life.h color.cpp color.h ledDraw.cpp ledDraw.h)

    target_link_libraries(rgb_matrix -lpthread)
endif ()
