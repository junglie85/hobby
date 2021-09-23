#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>
#include <glad/glad.h>

#include <stdarg.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

GLFWwindow* glfw_window = 0;
uint32_t screen_width = 1280;
uint32_t screen_height = 720;
const char* title = "Hobby";

static void glfw_error_callback(int32_t error, const char* description);

typedef enum LogLevel {
    LOG_LEVEL_TRACE,
    LOG_LEVEL_DEBUG,
    LOG_LEVEL_INFO,
    LOG_LEVEL_WARN,
    LOG_LEVEL_ERROR,
    LOG_LEVEL_FATAL,
} LogLevel;

static const char* log_level_to_string(LogLevel level);

static void log_message(LogLevel level, ...);

#define LOG_TRACE(...) log_message(LOG_LEVEL_TRACE, __VA_ARGS__)
#define LOG_DEBUG(...) log_message(LOG_LEVEL_DEBUG, __VA_ARGS__)
#define LOG_INFO(...) log_message(LOG_LEVEL_INFO, __VA_ARGS__)
#define LOG_WARN(...) log_message(LOG_LEVEL_WARN, __VA_ARGS__)
#define LOG_ERROR(...) log_message(LOG_LEVEL_ERROR, __VA_ARGS__)
#define LOG_FATAL(...) log_message(LOG_LEVEL_FATAL, __VA_ARGS__)

int main(void)
{
    glfwSetErrorCallback(glfw_error_callback);
    if (!glfwInit()) {
        LOG_FATAL("GLFW initialisation failed.");
        return EXIT_FAILURE;
    }

    glfwWindowHint(GLFW_VISIBLE, GLFW_FALSE);
    glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 1);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);

    glfw_window = glfwCreateWindow((int)screen_width, (int)screen_height, title, 0, 0);
    if (!glfw_window) {
        LOG_FATAL("GLFW window creation failed.");
        glfwTerminate();
        return EXIT_FAILURE;
    }

    glfwMakeContextCurrent(glfw_window);

    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
        LOG_FATAL("OpenGL initialisation failed.");
        glfwTerminate();
        return EXIT_FAILURE;
    }

    glfwShowWindow(glfw_window);

    while (!glfwWindowShouldClose(glfw_window)) {
        glViewport(0, 0, (int)screen_width, (int)screen_height);
        glClearColor(0.2f, 0.3f, 0.8f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        glfwPollEvents();
        glfwSwapBuffers(glfw_window);
    }

    glfwDestroyWindow(glfw_window);
    glfwTerminate();

    return EXIT_SUCCESS;
}

static void glfw_error_callback(int32_t error, const char* description)
{
    fprintf(stderr, "GLFW error (%d): %s\n", error, description);
}

static void log_message(LogLevel level, ...)
{
    FILE* out = stdout;
    if (level == LOG_LEVEL_ERROR || level == LOG_LEVEL_FATAL) {
        out = stderr;
    }

    char log_message[65535];
    memset(log_message, 0, sizeof(log_message));

    va_list args;
    va_start(args, level);
    const char* message = va_arg(args, char*);
    vsnprintf(log_message, sizeof(log_message), message, args);
    va_end(args);

    fprintf(out, "[%s]: %s\n", log_level_to_string(level), log_message);
}

static const char* log_level_to_string(LogLevel level)
{
    const char* strings[] = { "TRACE", "DEBUG", "INFO", "WARN", "ERROR", "FATAL" };
    return strings[level];
}
