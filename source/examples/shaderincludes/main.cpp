
#include <iostream>
#include <algorithm>

#include <cpplocate/cpplocate.h>

#include <glbinding/gl/gl.h>
#include <glbinding/Version.h>
#include <glbinding-aux/ContextInfo.h>
#include <glbinding-aux/types_to_string.h>

#include <glm/vec2.hpp>

#include <GLFW/glfw3.h>

#include <globjects/globjects.h>
#include <globjects/NamedString.h>
#include <globjects/base/AbstractStringSource.h>
#include <globjects/NamedString.h>
#include <globjects/Shader.h>
#include <globjects/logging.h>
#include <globjects/base/File.h>

#include "ScreenAlignedQuad.h"
#include "datapath.inl"


using namespace gl;


namespace
{
    const auto useFallback = false;

    std::unique_ptr<globjects::Program> g_program = nullptr;
    std::unique_ptr<globjects::AbstractStringSource> g_vertexShaderSource = nullptr;
    std::unique_ptr<globjects::AbstractStringSource> g_vertexShaderTemplate = nullptr;
    std::unique_ptr<globjects::Shader> g_vertexShader = nullptr;
    std::unique_ptr<globjects::File> g_fragmentShaderSource = nullptr;
    std::unique_ptr<globjects::AbstractStringSource> g_fragmentShaderTemplate = nullptr;
    std::unique_ptr<globjects::Shader> g_fragmentShader = nullptr;

    std::unique_ptr<globjects::File> g_namedStringSource = nullptr;
    std::unique_ptr<globjects::NamedString> g_namedString = nullptr;

    std::unique_ptr<ScreenAlignedQuad> g_quad = nullptr;

    auto g_size = glm::ivec2{};
}


void initialize()
{
    const auto dataPath = common::determineDataPath();

    g_namedStringSource = globjects::File::create(dataPath + "/shaderincludes/color.glsl");
    g_namedString = globjects::NamedString::create("/color.glsl", g_namedStringSource.get());

    g_vertexShaderSource = ScreenAlignedQuad::vertexShaderSource();
    g_vertexShaderTemplate = globjects::Shader::applyGlobalReplacements(g_vertexShaderSource.get());
    g_vertexShader = globjects::Shader::create(GL_VERTEX_SHADER, g_vertexShaderTemplate.get());

    g_fragmentShaderSource = globjects::Shader::sourceFromFile(dataPath + "/shaderincludes/test.frag");
    g_fragmentShaderTemplate = globjects::Shader::applyGlobalReplacements(g_fragmentShaderSource.get());
    g_fragmentShader = globjects::Shader::create(GL_FRAGMENT_SHADER, g_fragmentShaderTemplate.get());

    g_program = globjects::Program::create();
    g_program->attach(g_vertexShader.get(), g_fragmentShader.get());

    g_quad = ScreenAlignedQuad::create(g_program.get());
}

void deinitialize()
{
    g_program.reset(nullptr);
    g_vertexShader.reset(nullptr);
    g_vertexShaderTemplate.reset(nullptr);
    g_vertexShaderSource.reset(nullptr);
    g_fragmentShader.reset(nullptr);
    g_fragmentShaderTemplate.reset(nullptr);
    g_fragmentShaderSource.reset(nullptr);

    g_namedString.reset(nullptr);
    g_namedStringSource.reset(nullptr);

    g_quad.reset(nullptr);
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
        g_namedStringSource->reload();
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
    
    // Initialize GLFW
    if (!glfwInit())
    {
        globjects::critical() << "GLFW could not be initialized. Terminate execution.";

        return 1;
    }

    glfwSetErrorCallback(error);

    glfwDefaultWindowHints();
    glfwWindowHint(GLFW_VISIBLE, true);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, true);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
#ifdef SYSTEM_DARWIN
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 2);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
#else
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 1);
#endif

    // Create a context and, if valid, make it current
    GLFWwindow * window = glfwCreateWindow(640, 480, "globjects Shader Includes", nullptr, nullptr);
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
    if (useFallback)
    {
        globjects::init([](const char * name) {
            return glfwGetProcAddress(name);
        }, globjects::Shader::IncludeImplementation::Fallback);
    }
    else
    {
        globjects::init([](const char * name) {
            return glfwGetProcAddress(name);
        });
    }

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
