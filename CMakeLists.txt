cmake_minimum_required(VERSION 3.18.4)

project(Trianglo)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

list(APPEND GLAD_SRC "vendor/glad/src/glad.c")
list(APPEND GLAD_INCLUDE "vendor/glad/include")
list(APPEND GLM_INCLUDE "vendor/glm/")

file(GLOB_RECURSE SOURCES CONFIGURE_DEPENDS code/*.cpp)
add_executable("${PROJECT_NAME}" ${SOURCES} ${GLAD_SRC})

target_compile_options("${PROJECT_NAME}" PRIVATE -Wall -Wextra)

# Set to use C++17
set_target_properties(
	"${PROJECT_NAME}"
	PROPERTIES
	CXX_STANDARD 17
	)

target_include_directories(
	"${PROJECT_NAME}"
	PRIVATE
	header
	${GLAD_INCLUDE}
	${GLM_INCLUDE}
	)

target_link_libraries(
	"${PROJECT_NAME}"
	PRIVATE
	glfw
	GL
	X11
	pthread
	Xrandr
	Xi
	dl
	)
