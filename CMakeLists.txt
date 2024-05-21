cmake_minimum_required(VERSION 3.17)
project(rgb_matrix)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(SOURCES
        main.cpp
        game.h
        color.cpp
        color.h
        ants/ant.cpp
        ants/ant.h
        ants/ants.cpp
        ants/ants.h
        life/life.cpp
        life/life.h
        clock/clock.cpp
        clock/clock.h
        none/none.cpp
        none/none.h
)

if (CMAKE_SYSTEM_NAME MATCHES Darwin)
    add_compile_definitions(MACOS)
    add_executable(rgb_matrix ${SOURCES})
else ()
    add_executable(rgb_matrix ${SOURCES} ledDraw.cpp ledDraw.h)
    find_library(RGB_MATRIX_LIB rgbmatrix)
    target_link_libraries(rgb_matrix LINK_PUBLIC ${RGB_MATRIX_LIB} -lpthread -lpigpio -lrt)
endif ()
