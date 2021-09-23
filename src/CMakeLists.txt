find_package(OpenGL REQUIRED)
include_directories(${OPENGL_INCLUDE_DIRECTORIES})

add_subdirectory(${CMAKE_SOURCE_DIR}/extern/cglm ${CMAKE_CURRENT_BINARY_DIR}/extern/cglm)

add_subdirectory(${CMAKE_SOURCE_DIR}/extern/glad ${CMAKE_CURRENT_BINARY_DIR}/extern/glad)

set(GLFW_BUILD_DOCS OFF CACHE BOOL "GLFW lib only")
set(GLFW_BUILD_EXAMPLES OFF CACHE BOOL "GLFW lib only")
set(GLFW_BUILD_TESTS OFF CACHE BOOL "GLFW lib only")
set(GLFW_INSTALL OFF CACHE BOOL "GLFW lib only")
add_subdirectory(${CMAKE_SOURCE_DIR}/extern/glfw ${CMAKE_CURRENT_BINARY_DIR}/extern/glfw)

set(CMAKE_C_STANDARD 11)

set(hobby_public_headers
        )

set(hobby_sources
        macos_hobby.c
        )

add_executable(hobby ${hobby_sources} ${hobby_public_headers})

set_target_properties(hobby PROPERTIES
        PUBLIC_HEADER "${hobby_public_headers}"
        )

target_link_libraries(hobby PRIVATE ${OPENGL_LIBRARIES} cglm glad glfw)

target_include_directories(hobby
        PUBLIC
        $<INSTALL_INTERFACE:include>
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}>
        PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}
        )

file(COPY ${hobby_public_headers} DESTINATION "include")
