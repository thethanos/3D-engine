cmake_minimum_required(VERSION 3.0)

project(RealEngine)

set(CXX_STANDARD 20)
set(CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_FLAGS ${CMAKE_CXX_FLAGS} "-pthread")

aux_source_directory(src SOURCES)

add_executable(start main.cpp ${SOURCES})

include_directories(include)
target_link_libraries(start dl glfw)

find_package(assimp REQUIRED)
link_directories(${ASSIMP_LIBRARY_DIRS})
include_directories(${ASSIMP_INCLUDE_DIRS})
target_link_libraries(start ${ASSIMP_LIBRARIES})
