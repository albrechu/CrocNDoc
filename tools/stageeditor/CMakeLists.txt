cmake_minimum_required(VERSION 3.13) # If I remember correctly this ones not deprecated

project(stageeditor)

find_package(OpenGL REQUIRED)
include(external.cmake)

add_executable(stageeditor src/main.cpp)
target_link_libraries(stageeditor ${OPENGL_LIBRARIES} glad glfw imgui glm::glm nlohmann_json::nlohmann_json)
target_include_directories(stageeditor PRIVATE include ${OPENGL_INCLUDE_DIRS})
target_compile_definitions(stageeditor PRIVATE SHADERS_PATH="${CMAKE_SOURCE_DIR}/assets/shaders" ASSETS_PATH="${CMAKE_SOURCE_DIR}/assets")
set_target_properties(stageeditor PROPERTIES
	CXX_STANDARD 20
)
