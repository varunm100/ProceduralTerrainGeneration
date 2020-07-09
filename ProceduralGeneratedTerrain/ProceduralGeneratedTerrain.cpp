// ProceduralGeneratedTerrain.cpp : This file contains the 'main' function. Program execution begins and ends there.
//
#include <glad\glad.h>
#include <GLFW\glfw3.h>
#include <spdlog/spdlog.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>
#include "ShaderFileParser.h"
#include "VertexArray.h"
#include "Buffer.h"
#include "Shader.h"
#include "Camera.h"
#include "Terrain.h"


void APIENTRY glDebugCallback(GLenum source, GLenum mType, GLuint id, GLenum severity, GLsizei length, const GLchar* message, const void* userParam);
void framebuffer_size_callback(GLFWwindow* window, int width, int height);

int main() {
    GLFWwindow* window;
    const int width = 1920, height = 1080;
    const char* title = "Precedural Generated Terrain";

    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 5);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    window = glfwCreateWindow(width, height, title, NULL, NULL);
    if (window == NULL) {
        printf("Failed to Create Window :-(");
        glfwTerminate();
    }
    glfwSwapInterval(true); // vsync
    glfwMakeContextCurrent(window);
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) printf("Failed to initialize GLAD :-(");
    glViewport(0, 0, 1920, 1080);

    glEnable(GL_DEBUG_OUTPUT);
    glEnable(GL_DEBUG_OUTPUT_SYNCHRONOUS);
    glDebugMessageCallback(glDebugCallback, nullptr);

    glEnable(GL_DEPTH_TEST);

    Shader program("D:/CDEV/VisualStudioProjects/ProceduralGeneratedTerrain/ProceduralGeneratedTerrain/ProceduralGeneratedTerrain/Terrain.glsl");

    Terrain terrain(&program);
    terrain.calculateMesh(50, glm::vec3(0.0f), 1.0f);

    Camera cam(glm::vec3(0.0f, 5.0f, 0.0f), glm::vec3(0.0f, -1.0f, 0.0f));

    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
    glfwSetWindowUserPointer(window, &cam);
    glfwSetCursorPosCallback(window, [](GLFWwindow* window, double xpos, double ypos) {
        Camera* tcam = (Camera*)glfwGetWindowUserPointer(window);
        tcam->mouseCallback(window, xpos, ypos);
        });

    float time = glfwGetTime();
    float dt;
    glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
    while (!glfwWindowShouldClose(window))
    {
        // clear window
        glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        dt = glfwGetTime() - time;
        time = glfwGetTime();
        cam.checkInput(window, dt);
        // start here

        terrain.render(cam.view);

        //
        glfwSwapBuffers(window);
        glfwPollEvents();
    }
    glfwTerminate();
	return 0;
}

void  APIENTRY glDebugCallback(GLenum source, GLenum mType, GLuint id, GLenum severity, GLsizei length, const GLchar* message, const void* userParam)
{
    std::string src;
    switch (source) {
    case GL_DEBUG_SOURCE_API:
        src = "Windows";
        break;
    case GL_DEBUG_SOURCE_SHADER_COMPILER:
        src = "Shader Compiler";
        break;
    case GL_DEBUG_SOURCE_THIRD_PARTY:
        src = "Third Party";
        break;
    case GL_DEBUG_SOURCE_APPLICATION:
        src = "Application";
        break;
    case GL_DEBUG_SOURCE_OTHER:
        src = "Other";
        break;
    }

    std::string type;
    switch (mType) {
    case GL_DEBUG_TYPE_ERROR:
        type = "Error";
        break;
    case GL_DEBUG_TYPE_DEPRECATED_BEHAVIOR:
        type = "Deprecated Behavior";
        break;
    case GL_DEBUG_TYPE_UNDEFINED_BEHAVIOR:
        type = "Undefined Behavior";
        break;
    case GL_DEBUG_TYPE_PORTABILITY:
        type = "Portability Error";
        break;
    case GL_DEBUG_TYPE_PERFORMANCE:
        type = "Performance Error";
        break;
    case GL_DEBUG_TYPE_OTHER:
        type = "Other Message";
        break;
    }

    switch (severity) {
    case GL_DEBUG_SEVERITY_NOTIFICATION:
        spdlog::trace("GL NOTIFICATION - {} {}: {}", src, type, message);
        break;
    case GL_DEBUG_SEVERITY_LOW:
        spdlog::warn("GL LOW - {} {}: {}", src, type, message);
        break;
    case GL_DEBUG_SEVERITY_MEDIUM:
        spdlog::error("GL MEDIUM - {} {}: {}", src, type, message);
        break;
    case GL_DEBUG_SEVERITY_HIGH:
        spdlog::critical("GL HIGH - {} {}: {}", src, type, message);
        break;
    }
    return;
}

void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
    // make sure the viewport matches the new window dimensions; note that width and 
    // height will be significantly larger than specified on retina displays.
    glViewport(0, 0, width, height);
}