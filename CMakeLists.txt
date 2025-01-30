cmake_minimum_required(VERSION 3.7)
project(hello_world_sdl2)
set(SDL2_DIR lib/SDL2-2.30.3/cmake)
find_package(SDL2 REQUIRED)
include_directories(${SDL2_INCLUDE_DIRS})
set(SOURCE_DIR src)
set(INCLUDE_DIR include)

# GLAD .h
set(GLAD_DIR ${INCLUDE_DIR}/glad)
include_directories(${GLAD_DIR}/include)

set(SOURCES 
    ${SOURCE_DIR}/main.cpp 
    ${SOURCE_DIR}/app.cpp
    ${GLAD_DIR}/src/glad.c
)
add_executable(a.out ${SOURCES})
target_include_directories(a.out PRIVATE ${SOURCE_DIR})
target_link_libraries(a.out)
target_link_libraries(a.out ${SDL2_LIBRARIES})