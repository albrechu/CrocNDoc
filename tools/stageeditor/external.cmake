include(FetchContent)

set(EXTERNAL_DIR ${CMAKE_BINARY_DIR}/_deps/build) # Where the library builds are stored

FetchContent_Declare(
	#[[TARGET]]    glfw
	GIT_REPOSITORY https://github.com/glfw/glfw.git
	GIT_TAG        3.4
)

FetchContent_Declare(
	#[[TARGET]]    imgui
	GIT_REPOSITORY https://github.com/ocornut/imgui.git
	GIT_TAG        v1.91.8-docking
	GIT_SHALLOW	   true # depth=1
	SOURCE_SUBDIR  cmake
)

FetchContent_Declare(
	#[[TARGET]]    glm
	GIT_REPOSITORY https://github.com/g-truc/glm.git
	GIT_TAG 	   bf71a834948186f4097caa076cd2663c69a10e1e #refs/tags/1.0.1
)

FetchContent_Declare(
	#[[TARGET]] json 
	URL         https://github.com/nlohmann/json/releases/download/v3.12.0/json.tar.xz
)

FetchContent_MakeAvailable(json glfw imgui glm)

## Add glfw
#add_subdirectory("${glfw_SOURCE_DIR}" "${glfw_BINARY_DIR}")

## Add imgui
add_library(imgui STATIC 
	# Core
	"${imgui_SOURCE_DIR}/imconfig.h"
	"${imgui_SOURCE_DIR}/imgui_internal.h"
	"${imgui_SOURCE_DIR}/imgui_draw.cpp"
	"${imgui_SOURCE_DIR}/imgui_tables.cpp"
	"${imgui_SOURCE_DIR}/imgui_widgets.cpp"
	"${imgui_SOURCE_DIR}/imstb_rectpack.h"
	"${imgui_SOURCE_DIR}/imstb_textedit.h"
	"${imgui_SOURCE_DIR}/imstb_truetype.h"
	"${imgui_SOURCE_DIR}/imgui.cpp" "${imgui_SOURCE_DIR}/imgui.h"
	# Backends
	"${imgui_SOURCE_DIR}/backends/imgui_impl_glfw.cpp"   "${imgui_SOURCE_DIR}/backends/imgui_impl_glfw.h"
	"${imgui_SOURCE_DIR}/backends/imgui_impl_opengl3.cpp" "${imgui_SOURCE_DIR}/backends/imgui_impl_opengl3.h"
)
target_include_directories(imgui PUBLIC ${imgui_SOURCE_DIR})
target_link_libraries(imgui PUBLIC glfw)

## Add glad
add_library(glad INTERFACE )
