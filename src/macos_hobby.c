#include <cglm/cglm.h>
#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>
#include <glad/glad.h>

#include <stdarg.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

static GLFWwindow* glfw_window = 0;
static uint32_t screen_width = 1280;
static uint32_t screen_height = 720;
static const char* title = "Hobby";
static GLuint shader_program = 0;
uint32_t vertex_array = 0;
uint32_t vertex_buffer = 0;
uint32_t index_buffer = 0;

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

static void opengl_init(void);
static void opengl_shutdown(void);
static void draw_box(vec2 p1, vec2 p2, vec4 color);

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

    opengl_init();

    glfwShowWindow(glfw_window);

    while (!glfwWindowShouldClose(glfw_window)) {
        glViewport(0, 0, (int)screen_width, (int)screen_height);
        glClearColor(0.2f, 0.3f, 0.8f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        draw_box((vec2) { 0.0f, 0.0f }, (vec2) { 10.0f, 10.0f }, (vec4) { 0.7f, 0.6f, 0.0f, 1.0f });

        glfwPollEvents();
        glfwSwapBuffers(glfw_window);
    }

    opengl_shutdown();

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

static void opengl_init(void)
{
    const char* vertex_source = "#version 410 core\n"
                                "layout (location = 0) in vec2 a_Position;\n"
                                "void main() {\n"
                                "    gl_Position = vec4(a_Position, 0.0, 1.0);\n"
                                "}\n";

    const char* fragment_source = "#version 410 core\n"
                                  "out vec4 f_Color;\n"
                                  "uniform vec4 u_Color;"
                                  "void main() {\n"
                                  "    f_Color = u_Color;\n"
                                  "}\n";

    GLuint vertex_shader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertex_shader, 1, &vertex_source, 0);
    glCompileShader(vertex_shader);

    GLint is_compiled = 0;
    glGetShaderiv(vertex_shader, GL_COMPILE_STATUS, &is_compiled);
    if (is_compiled == GL_FALSE) {
        GLchar info_log[65535];
        glGetShaderInfoLog(vertex_shader, sizeof(info_log), 0, info_log);

        glDeleteShader(vertex_shader);

        LOG_ERROR("Vertex shader compilation error: %s", info_log);
        // todo: ASSERT?
        return;
    }

    // Create an empty fragment shader handle
    GLuint fragment_shader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragment_shader, 1, &fragment_source, 0);
    glCompileShader(fragment_shader);

    glGetShaderiv(fragment_shader, GL_COMPILE_STATUS, &is_compiled);
    if (is_compiled == GL_FALSE) {
        GLchar info_log[65535];
        glGetShaderInfoLog(fragment_shader, sizeof(info_log), 0, info_log);

        glDeleteShader(fragment_shader);
        glDeleteShader(vertex_shader);

        LOG_ERROR("Fragment shader compilation error: %s", info_log);
        // todo: ASSERT?
        return;
    }

    shader_program = glCreateProgram();
    glAttachShader(shader_program, vertex_shader);
    glAttachShader(shader_program, fragment_shader);
    glLinkProgram(shader_program);

    GLint is_linked = 0;
    glGetProgramiv(shader_program, GL_LINK_STATUS, &is_linked);
    if (is_linked == GL_FALSE) {
        GLchar info_log[65535];
        glGetProgramInfoLog(shader_program, sizeof(info_log), 0, info_log);

        glDeleteProgram(shader_program);
        glDeleteShader(vertex_shader);
        glDeleteShader(fragment_shader);

        LOG_ERROR("Shader program linking error: %s", info_log);
        // todo: ASSERT?
        return;
    }

    glDetachShader(shader_program, vertex_shader);
    glDetachShader(shader_program, fragment_shader);

    glGenVertexArrays(1, &vertex_array);
    glBindVertexArray(vertex_array);

    // clang-format off
    float vertices[] = {
        -0.5f, 0.5f,
        -0.5f, -0.5f,
        0.5f, -0.5f,
        0.5f, 0.5f,
    };
    // clang-format on

    glGenBuffers(1, &vertex_buffer);
    glBindBuffer(GL_ARRAY_BUFFER, vertex_buffer);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 2, GL_FLOAT, false, 2 * sizeof(float), 0);

    // clang-format off
    uint32_t indices[] = {
        0, 1, 2,
        0, 2, 3,
    };
    // clang-format on

    glGenBuffers(1, &index_buffer);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, index_buffer);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);
}

static void opengl_shutdown(void)
{
    glDeleteBuffers(1, &index_buffer);
    glDeleteBuffers(1, &vertex_buffer);
    glDeleteVertexArrays(1, &vertex_array);
    glDeleteProgram(shader_program);
}

static void draw_box(vec2 p1, vec2 p2, vec4 color)
{
    glUseProgram(shader_program);
    int32_t location = glGetUniformLocation(shader_program, "u_Color");
    glUniform4fv(location, 1, &color[0]);
    glBindVertexArray(vertex_array);
    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
    glBindVertexArray(0);
    glUseProgram(0);
}
