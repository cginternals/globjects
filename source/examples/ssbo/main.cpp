
#include <iostream>
#include <algorithm>
#include <array>

#include <glm/vec2.hpp>

#include <cpplocate/cpplocate.h>
#include <cpplocate/ModuleInfo.h>

#include <glbinding/gl/gl.h>
#include <glbinding/gl/extension.h>
#include <glbinding/ContextInfo.h>
#include <glbinding/Version.h>

#include <GLFW/glfw3.h>

#include <globjects/globjects.h>
#include <globjects/base/File.h>
#include <globjects/logging.h>

#include <globjects/Buffer.h>
#include <globjects/Program.h>

#include "ScreenAlignedQuad.h"
#include "datapath.inl"


using namespace gl;


namespace 
{
    ScreenAlignedQuad * g_quad = nullptr;
    globjects::Buffer * g_buffer = nullptr;

    auto g_size = glm::ivec2{};
}


void initialize()
{
    const auto dataPath = common::retrieveDataPath("globjects", "dataPath");
    g_quad = new ScreenAlignedQuad(globjects::Shader::fromFile(GL_FRAGMENT_SHADER, dataPath + "ssbo/ssbo.frag"));
    g_quad->ref();

    g_quad->program()->setUniform("maximum",     10);
    g_quad->program()->setUniform("rowCount",    10);
    g_quad->program()->setUniform("columnCount", 10);

    static const auto data = std::array<int, 100> {
        1,2,3,4,5,6,7,8,9,10,
        10,1,2,3,4,5,6,7,8,9,
        9,10,1,2,3,4,5,6,7,8,
        8,9,10,1,2,3,4,5,6,7,
        7,8,9,10,1,2,3,4,5,6,
        6,7,8,9,10,1,2,3,4,5,
        5,6,7,8,9,10,1,2,3,4,
        4,5,6,7,8,9,10,1,2,3,
        3,4,5,6,7,8,9,10,1,2,
        2,3,4,5,6,7,8,9,10,1 };

    g_buffer = new globjects::Buffer();
    g_buffer->ref();
    g_buffer->setData(sizeof(data), data.data(), GL_STATIC_DRAW);

    g_buffer->bindBase(GL_SHADER_STORAGE_BUFFER, 1);
}

void deinitialize()
{
    g_quad->unref();
    g_buffer->unref();
}

void draw()
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glViewport(0, 0, g_size.x, g_size.y);
    g_quad->draw();
}


void error(int errnum, const char * errmsg)
{
    globjects::critical() << errnum << ": " << errmsg << std::endl;
}

void framebuffer_size_callback(GLFWwindow * /*window*/, int width, int height)
{
    g_size = glm::ivec2{ width, height };
}

void key_callback(GLFWwindow * window, int key, int /*scancode*/, int action, int /*modes*/)
{
    if (key == GLFW_KEY_ESCAPE && action == GLFW_RELEASE)
        glfwSetWindowShouldClose(window, true);

    if (key == GLFW_KEY_F5 && action == GLFW_RELEASE)
        globjects::File::reloadAll();
}


int main(int /*argc*/, char * /*argv*/[])
{
    // Initialize GLFW
    if (!glfwInit())
        return 1;

    glfwSetErrorCallback(error);
    glfwDefaultWindowHints();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 1);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, true);

    // Create a context and, if valid, make it current
    GLFWwindow * window = glfwCreateWindow(640, 480, "globjects Shader Storage Buffer Objects", NULL, NULL);
    if (window == nullptr)
    {
        globjects::critical() << "Context creation failed. Terminate execution.";

        glfwTerminate();
        return -1;
    }
    glfwSetKeyCallback(window, key_callback);
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

    glfwMakeContextCurrent(window);

    // Initialize globjects (internally initializes glbinding, and registers the current context)
    globjects::init();

    std::cout << std::endl
        << "OpenGL Version:  " << glbinding::ContextInfo::version() << std::endl
        << "OpenGL Vendor:   " << glbinding::ContextInfo::vendor() << std::endl
        << "OpenGL Renderer: " << glbinding::ContextInfo::renderer() << std::endl << std::endl;

    globjects::info() << "Press F5 to reload shaders." << std::endl << std::endl;


    glfwGetFramebufferSize(window, &g_size[0], &g_size[1]);
    initialize();

    // Main loop
    while (!glfwWindowShouldClose(window))
    {
        glfwPollEvents();
        draw();
        glfwSwapBuffers(window);
    }
    deinitialize();

    // Properly shutdown GLFW
    glfwTerminate();

    return 0;
}
