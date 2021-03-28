cmake_minimum_required(VERSION 3.17)
project(rgb_matrix)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

#if (CMAKE_BUILD_TYPE MATCHES Debug)
#    add_compile_definitions(DEBUG)
#    add_executable(rgb_matrix main.cpp ants/ant.cpp ants/ant.h game.h life/life.cpp life/life.h color.cpp color.h ants/ants.cpp ants/ants.h)
#else ()
#include_directories(${PROJECT_SOURCE_DIR}/matrix/include)
#include_directories(${PROJECT_SOURCE_DIR}/matrix/lib)
#
#link_directories(${PROJECT_SOURCE_DIR}/matrix/lib)

#add_library(matrix STATIC ${PROJECT_SOURCE_DIR}/matrix/lib)
#set_target_properties(matrix PROPERTIES LINKER_LANGUAGE CXX)


#include_directories(${PROJECT_SOURCE_DIR}/matrix/lib)
#include_directories(${PROJECT_SOURCE_DIR}/matrix/include)
#file(GLOB_RECURSE RGB_SRC_FILES ${PROJECT_SOURCE_DIR}/matrix/lib/*.cc)
#file(GLOB_RECURSE RGB_HEADER_FILES ${PROJECT_SOURCE_DIR}/matrix/include/*.h)

find_library(RGB_MATRIX_LIB rgbmatrix
        NAMES librgbmatrix
        HINTS "/usr/lib/rgbmatrix"
        )
if (NOT RGB_MATRIX_LIB)
    message(FATAL_ERROR "RGB matrix library not found ")
endif ()

#add_executable(rgb_matrix ${RGB_SRC_FILES} ${RGB_HEADER_FILES} ${RGB_MORE_HEADER_FILES} main.cpp ants/ant.cpp ants/ant.h ants/ants.cpp ants/ants.h game.h life/life.cpp life/life.h color.cpp color.h ledDraw.cpp ledDraw.h)
add_executable(rgb_matrix ${RGB_SRC_FILES} ${RGB_HEADER_FILES} main.cpp ants/ant.cpp ants/ant.h ants/ants.cpp ants/ants.h game.h life/life.cpp life/life.h color.cpp color.h ledDraw.cpp ledDraw.h)
target_link_libraries(rgb_matrix -lpthread)

#endif ()
