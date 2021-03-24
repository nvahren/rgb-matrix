cmake_minimum_required(VERSION 3.17)
project(rgb_matrix)

set(CMAKE_CXX_STANDARD 20)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -c")

include_directories(${CMAKE_SOURCE_DIR}/matrix/include)
link_directories(${CMAKE_SOURCE_DIR}/matrix/lib)

add_executable(rgb_matrix main.cpp ant.cpp ant.h ants.cpp game.h life.cpp life.h color.cpp color.h)