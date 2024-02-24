
#include <iostream>
#include <algorithm>
#include <array>

#include <glm/vec2.hpp>

#include <cpplocate/cpplocate.h>

#include <glbinding/gl/gl.h>
#include <glbinding/gl/extension.h>
#include <glbinding/Version.h>
#include <glbinding-aux/ContextInfo.h>
#include <glbinding-aux/types_to_string.h>

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
    std::unique_ptr<ScreenAlignedQuad> g_quad = nullptr;
    std::unique_ptr<globjects::Buffer> g_buffer = nullptr;

    std::unique_ptr<globjects::Program> g_program = nullptr;
    std::unique_ptr<globjects::AbstractStringSource> g_vertexShaderSource = nullptr;
    std::unique_ptr<globjects::AbstractStringSource> g_vertexShaderTemplate = nullptr;
    std::unique_ptr<globjects::Shader> g_vertexShader = nullptr;
    std::unique_ptr<globjects::File> g_fragmentShaderSource = nullptr;
    std::unique_ptr<globjects::AbstractStringSource> g_fragmentShaderTemplate = nullptr;
    std::unique_ptr<globjects::Shader> g_fragmentShader = nullptr;

    auto g_size = glm::ivec2{};
}


void initialize()
{
    const auto dataPath = common::determineDataPath();

    g_vertexShaderSource = ScreenAlignedQuad::vertexShaderSource();
    g_vertexShaderTemplate = globjects::Shader::applyGlobalReplacements(g_vertexShaderSource.get());
    g_vertexShader = globjects::Shader::create(GL_VERTEX_SHADER, g_vertexShaderTemplate.get());

    g_fragmentShaderSource = globjects::Shader::sourceFromFile(dataPath + "/ssbo/ssbo.frag");
    g_fragmentShaderTemplate = globjects::Shader::applyGlobalReplacements(g_fragmentShaderSource.get());
    g_fragmentShader = globjects::Shader::create(GL_FRAGMENT_SHADER, g_fragmentShaderTemplate.get());

    g_program = globjects::Program::create();
    g_program->attach(g_vertexShader.get(), g_fragmentShader.get());

    g_quad = ScreenAlignedQuad::create(g_program.get());

    g_program->setUniform("maximum",     10);
    g_program->setUniform("rowCount",    10);
    g_program->setUniform("columnCount", 10);

    static const auto data = std::array<int, 100> {{
        1,2,3,4,5,6,7,8,9,10,
        10,1,2,3,4,5,6,7,8,9,
        9,10,1,2,3,4,5,6,7,8,
        8,9,10,1,2,3,4,5,6,7,
        7,8,9,10,1,2,3,4,5,6,
        6,7,8,9,10,1,2,3,4,5,
        5,6,7,8,9,10,1,2,3,4,
        4,5,6,7,8,9,10,1,2,3,
        3,4,5,6,7,8,9,10,1,2,
        2,3,4,5,6,7,8,9,10,1 }};

    g_buffer = globjects::Buffer::create();
    g_buffer->setData(sizeof(data), data.data(), GL_STATIC_DRAW);

    g_buffer->bindBase(GL_SHADER_STORAGE_BUFFER, 1);
}

void deinitialize()
{
    g_quad.reset(nullptr);
    g_buffer.reset(nullptr);

    g_program.reset(nullptr);
    g_vertexShader.reset(nullptr);
    g_vertexShaderTemplate.reset(nullptr);
    g_vertexShaderSource.reset(nullptr);
    g_fragmentShader.reset(nullptr);
    g_fragmentShaderTemplate.reset(nullptr);
    g_fragmentShaderSource.reset(nullptr);
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
    {
        g_fragmentShaderSource->reload();
    }
}

void glfwError(int error_code, const char *description)
{
    globjects::info() << "glfw3 (" << error_code << "): " << description << std::endl;
}


int main(int /*argc*/, char * /*argv*/[])
{
    glfwSetErrorCallback(glfwError);
    
#ifdef SYSTEM_DARWIN
    globjects::critical() << "macOS does currently not support shader storage buffer objects (OpenGL 4.3. required).";
    return 0;
#endif

    // Initialize GLFW
    if (!glfwInit())
    {
        globjects::critical() << "GLFW could not be initialized. Terminate execution.";

        return 1;
    }

    glfwSetErrorCallback(error);
    glfwDefaultWindowHints();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, true);

    // Create a context and, if valid, make it current
    GLFWwindow * window = glfwCreateWindow(640, 480, "globjects Shader Storage Buffer Objects", nullptr, nullptr);
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
    globjects::init([](const char * name) {
        return glfwGetProcAddress(name);
    });

    std::cout << std::endl
        << "OpenGL Version:  " << glbinding::aux::ContextInfo::version() << std::endl
        << "OpenGL Vendor:   " << glbinding::aux::ContextInfo::vendor() << std::endl
        << "OpenGL Renderer: " << glbinding::aux::ContextInfo::renderer() << std::endl << std::endl;

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
