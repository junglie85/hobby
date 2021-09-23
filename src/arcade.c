#include <arcade.h>

#include <cglm/cglm.h>
#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>
#include <glad/glad.h>

#include <stdint.h>
#include <stdio.h>

GLFWwindow* glfw_window = 0;
const uint32_t screen_width = 1280;
const uint32_t screen_height = 720;
const char* title = "Arcade";

static void glfw_error_callback(int32_t error, const char* description);
static void initialise(void);
static void update(void);
static void shutdown(void);

int arcade_run(void)
{
    initialise();

    while (!glfwWindowShouldClose(glfw_window)) {
        update();

        glfwPollEvents();
        glfwSwapBuffers(glfw_window);
    }

    shutdown();

    return EXIT_SUCCESS;
}

static void glfw_error_callback(int32_t error, const char* description)
{
    fprintf(stderr, "%s\n", description);
}

static void initialise(void)
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

    glfw_window = glfwCreateWindow((int)screen_width, (int)screen_height, title, 0, 0);
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

    glfwShowWindow(glfw_window);
}

static void update(void)
{
    glViewport(0, 0, (int)screen_width, (int)screen_height);
    glClearColor(1.0f, 0.1f, 1.0f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

static void shutdown(void)
{
    glfwDestroyWindow(glfw_window);
    glfwTerminate();
}
