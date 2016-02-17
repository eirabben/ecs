cmake_minimum_required(VERSION 3.4 FATAL_ERROR)
project(Ecs VERSION 0.2 LANGUAGES CXX)

file(GLOB_RECURSE ECS_SRC "*.hpp" "*.cpp")
add_library(ecs ${ECS_SRC})

#target_include_directories(ecs PUBLIC
#${CMAKE_CURRENT_SOURCE_DIR}/include
#PRIVATE src)

#set_property(TARGET ecs PROPERTY CXX_STANDARD 14)

#install(TARGETS ecs
#ARCHIVE DESTINATION lib
#LIBRARY DESTINATION lib
#RUNTIME DESTINATION bin)
#install(DIRECTORY include/ DESTINATION include)

#export(TARGETS ecs FILE EcsConfig.cmake)

