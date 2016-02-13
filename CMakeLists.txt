cmake_minimum_required(VERSION 3.4)
project(Ecs VERSION 0.2 LANGUAGES CXX)

# @TODO: Probably need to add more
add_library(Ecs
    src/TypeManager.cpp)

target_include_directories(Ecs PUBLIC
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
    $<INSTALL_INTERFACE:include>
    PRIVATE src)

# Use one of these:
target_compile_features(Ecs
    PUBLIC cxx_auto_type
    PRIVATE cxx_variadic_templates)
# set(TARGET Ecs PROPERTY CXX_STANDARD 14)

install(TARGETS Ecs
    ARCHIVE DESTINATION lib
    LIBRARY DESTINATION lib
    RUNTIME DESTINATION bin)
install(DIRECTORY include/ DESTINATION include)

export(TARGETS Ecs FILE EcsConfig.cmake)

