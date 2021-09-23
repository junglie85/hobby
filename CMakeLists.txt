cmake_minimum_required(VERSION 3.20)
project(arcade C)

find_package(OpenGL REQUIRED)
include_directories(${OPENGL_INCLUDE_DIRECTORIES})

add_subdirectory(extern/cglm)

add_subdirectory(extern/glad)

set(GLFW_BUILD_DOCS OFF CACHE BOOL "GLFW lib only")
set(GLFW_BUILD_EXAMPLES OFF CACHE BOOL "GLFW lib only")
set(GLFW_BUILD_TESTS OFF CACHE BOOL "GLFW lib only")
set(GLFW_INSTALL OFF CACHE BOOL "GLFW lib only")
add_subdirectory(extern/glfw)

set(CMAKE_C_STANDARD 11)

add_executable(arcade main.c)
target_link_libraries(arcade ${OPENGL_LIBRARIES} cglm glad glfw)
