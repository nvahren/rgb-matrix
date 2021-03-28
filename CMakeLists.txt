cmake_minimum_required(VERSION 3.17)
project(rgb_matrix)

set(CMAKE_CXX_STANDARD 20)

#if (CMAKE_BUILD_TYPE MATCHES Debug)
#    add_compile_definitions(DEBUG)
#    add_executable(rgb_matrix main.cpp ants/ant.cpp ants/ant.h game.h life/life.cpp life/life.h color.cpp color.h ants/ants.cpp ants/ants.h)
#else ()
include_directories(${PROJECT_SOURCE_DIR}/matrix/include)
include_directories(${PROJECT_SOURCE_DIR}/matrix/lib)

link_directories(${PROJECT_SOURCE_DIR}/matrix/lib)

#add_library(matrix STATIC ${PROJECT_SOURCE_DIR}/matrix/lib)
#set_target_properties(matrix PROPERTIES LINKER_LANGUAGE CXX)


include(ExternalProject)
ExternalProject_Add(project_matrix
        SOURCE_DIR ${PROJECT_SOURCE_DIR}/matrix/lib
        CONFIGURE_COMMAND ""
        BUILD_COMMAND make
        INSTALL_COMMAND make install
        )
ExternalProject_Get_Property(project_matrix install_dir)
add_library(matrix STATIC IMPORTED)
set_property(TARGET matrix PROPERTY IMPORTED_LOCATION ${install_dir}/lib/libmatrix.a)
add_dependencies(matrix project_matrix)


#add_executable(rgb_matrix ${RGB_SRC_FILES} ${RGB_HEADER_FILES} ${RGB_MORE_HEADER_FILES} main.cpp ants/ant.cpp ants/ant.h ants/ants.cpp ants/ants.h game.h life/life.cpp life/life.h color.cpp color.h ledDraw.cpp ledDraw.h)
add_executable(rgb_matrix main.cpp ants/ant.cpp ants/ant.h ants/ants.cpp ants/ants.h game.h life/life.cpp life/life.h color.cpp color.h ledDraw.cpp ledDraw.h)
include_directories(${install_dir}/include/matrix)
target_link_libraries(rgb_matrix matrix)
#endif ()
