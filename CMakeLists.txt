cmake_minimum_required(VERSION 3.17)
project(rgb_matrix)

set(CMAKE_CXX_STANDARD 20)

#set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -c")

if (!DEBUG)
    include_directories(${CMAKE_SOURCE_DIR}/matrix/include)
    link_directories(${CMAKE_SOURCE_DIR}/matrix/lib)
    add_executable(rgb_matrix main.cpp ants/ant.cpp ants/ant.h game.h life/life.cpp life/life.h color.cpp color.h ledDraw.cpp ledDraw.h)
else ()
    add_compile_definitions(DEBUG)
    add_executable(rgb_matrix main.cpp ants/ant.cpp ants/ant.h game.h life/life.cpp life/life.h color.cpp color.h ants/ants.cpp ants/ants.h)
endif ()
