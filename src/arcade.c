#include <arcade.h>

#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>
#include <glad/glad.h>

#include <stdint.h>
#include <stdio.h>

GLFWwindow* glfw_window = 0;

static void glfw_error_callback(int32_t error, const char* description);

void arcade_window_create(uint32_t width, uint32_t height, const char* title)
{
    glfwSetErrorCallback(glfw_error_callback);
    if (!glfwInit()) {
        fprintf(stderr, "GLFW initialisation failed.");
        exit(EXIT_FAILURE);
    }

    glfwWindowHint(GLFW_VISIBLE, GLFW_FALSE);
    glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 1);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);

    glfw_window = glfwCreateWindow((int)width, (int)height, title, 0, 0);
    if (!glfw_window) {
        fprintf(stderr, "GLFW window creation failed.");
        glfwTerminate();
        exit(EXIT_FAILURE);
    }

    glfwMakeContextCurrent(glfw_window);

    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
        fprintf(stderr, "OpenGL initialisation failed.");
        glfwTerminate();
        exit(EXIT_FAILURE);
    }

    glViewport(0, 0, (int)width, (int)height);

    glfwShowWindow(glfw_window);
}

void arcade_set_clear_color(vec4 color)
{
    glClearColor(color[0], color[1], color[2], color[3]);
}

bool arcade_run(void)
{
    glfwPollEvents();
    glfwSwapBuffers(glfw_window);

    return !glfwWindowShouldClose(glfw_window);
}

void arcade_begin_render(void)
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

void arcade_window_destroy(void)
{
    glfwDestroyWindow(glfw_window);
    glfwTerminate();
}

static void glfw_error_callback(int32_t error, const char* description)
{
    fprintf(stderr, "GLFW error (%d): %s\n", error, description);
}
