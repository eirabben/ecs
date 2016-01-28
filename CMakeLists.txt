cmake_minimum_required(VERSION 2.6)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++14")

project(Ecs)
set(Ecs_VERSION_MAJOR 0)
set(Ecs_VERSION_MINOR 1)

set(PROJECT_SOURCE_DIR ${CMAKE_SOURCE_DIR}/src)

file(GLOB_RECURSE ECS_SRC "${PROJECT_SOURCE_DIR}/*.hpp" "${PROJECT_SOURCE_DIR}/*.cpp")
add_library(Ecs ${ECS_SRC})
