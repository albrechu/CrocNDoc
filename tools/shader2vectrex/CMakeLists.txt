cmake_minimum_required(VERSION 3.13) # If I remember correctly this ones not deprecated

project(shader2vectrex)


find_package(OpenGL REQUIRED)
include(external.cmake)

add_executable(shader2vectrex src/main.cpp )
target_link_libraries(shader2vectrex ${OPENGL_LIBRARIES} glad glfw imgui)
target_include_directories(shader2vectrex PRIVATE include ${OPENGL_INCLUDE_DIRS})
target_compile_definitions(shader2vectrex PRIVATE SHADERS_PATH="${CMAKE_SOURCE_DIR}/shaders")
