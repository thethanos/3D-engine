cmake_minimum_required(VERSION 3.0)

project(RealEngine)

set(CXX_STANDARD 20)
set(CXX_STANDARD_REQUIRED ON)

if(MINGW OR UNIX)
    set(CMAKE_CXX_FLAGS ${CMAKE_CXX_FLAGS} "-pthread")
endif()

aux_source_directory(src SOURCES)

add_executable(start main.cpp ${SOURCES})

option(BUILD_SHARED_LIBS "Build package with shared libraries." OFF)

add_subdirectory(libs/assimp)
add_subdirectory(libs/glfw)

include_directories(include libs/assimp/include libs/glfw/include)
target_link_libraries(start glfw assimp)
