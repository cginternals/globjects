
#include <iostream>
#include <algorithm>

#include <cpplocate/cpplocate.h>
#include <cpplocate/ModuleInfo.h>

#include <glm/vec2.hpp>

#include <glbinding/gl/gl.h>
#include <glbinding/ContextInfo.h>
#include <glbinding/Version.h>

#include <GLFW/glfw3.h>

#include <globjects/globjects.h>
#include <globjects/base/File.h>
#include <globjects/logging.h>

#include <globjects/Buffer.h>
#include <globjects/Program.h>
#include <globjects/Shader.h>
#include <globjects/VertexArray.h>
#include <globjects/VertexAttributeBinding.h>
#include <globjects/State.h>

#include "datapath.inl"


using namespace gl;


namespace 
{
    globjects::Program * g_shaderProgram = nullptr;
    globjects::VertexArray * g_vao = nullptr;
    globjects::Buffer * g_buffer = nullptr;
    globjects::State * g_thinnestPointSizeState = nullptr;
    globjects::State * g_thinPointSizeState = nullptr;
    globjects::State * g_normalPointSizeState = nullptr;
    globjects::State * g_thickPointSizeState = nullptr;
    globjects::State * g_disableRasterizerState = nullptr;
    globjects::State * g_enableRasterizerState = nullptr;
    globjects::State * g_defaultPointSizeState = nullptr;

    auto g_size = glm::ivec2{};
}


void initialize()
{
    // Initialize OpenGL objects
    g_defaultPointSizeState = new globjects::State();
    g_defaultPointSizeState->ref();
    g_defaultPointSizeState->pointSize(globjects::getFloat(GL_POINT_SIZE));
    g_thinnestPointSizeState = new globjects::State();
    g_thinnestPointSizeState->ref();
    g_thinnestPointSizeState->pointSize(2.0f);
    g_thinPointSizeState = new globjects::State();
    g_thinPointSizeState->ref();
    g_thinPointSizeState->pointSize(5.0f);
    g_normalPointSizeState = new globjects::State();
    g_normalPointSizeState->ref();
    g_normalPointSizeState->pointSize(10.0f);
    g_thickPointSizeState = new globjects::State();
    g_thickPointSizeState->ref();
    g_thickPointSizeState->pointSize(20.0f);
    g_disableRasterizerState = new globjects::State();
    g_disableRasterizerState->ref();
    g_disableRasterizerState->enable(GL_RASTERIZER_DISCARD);
    g_enableRasterizerState = new globjects::State();
    g_enableRasterizerState->ref();
    g_enableRasterizerState->disable(GL_RASTERIZER_DISCARD);

    g_vao = new globjects::VertexArray();
    g_vao->ref();
    g_buffer = new globjects::Buffer();
    g_buffer->ref();

    g_shaderProgram = new globjects::Program();
    g_shaderProgram->ref();

    const auto dataPath = common::retrieveDataPath("globjects", "dataPath");
    g_shaderProgram->attach(
        globjects::Shader::fromFile(GL_VERTEX_SHADER, dataPath + "states/standard.vert")
      , globjects::Shader::fromFile(GL_FRAGMENT_SHADER, dataPath + "states/standard.frag"));
    
    static auto data = std::vector<glm::vec2>(); 
    if (data.empty())
    {
        for (auto y = 0.8f; y > -1.f; y -= 0.2f)
            for (auto x = -0.8f; x < 1.f; x += 0.4f)
                data.push_back(glm::vec2(x, y));
    }
    g_buffer->setData(data, GL_STATIC_DRAW );

    g_vao->binding(0)->setAttribute(0);
    g_vao->binding(0)->setBuffer(g_buffer, 0, sizeof(glm::vec2));
    g_vao->binding(0)->setFormat(2, GL_FLOAT);
    g_vao->enable(0);
}

void deinitialize()
{
    g_shaderProgram->unref();
    g_vao->unref();
    g_buffer->unref();
    g_thinnestPointSizeState->unref();
    g_thinPointSizeState->unref();
    g_normalPointSizeState->unref();
    g_thickPointSizeState->unref();
    g_disableRasterizerState->unref();
    g_enableRasterizerState->unref();
    g_defaultPointSizeState->unref();

    globjects::detachAllObjects();
}

void draw()
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glViewport(0, 0, g_size.x, g_size.y);

    g_shaderProgram->use();

    g_defaultPointSizeState->apply();
    g_vao->drawArrays(GL_POINTS, 0, 5);

    g_thinnestPointSizeState->apply();
    g_vao->drawArrays(GL_POINTS, 5, 5);

    g_thinPointSizeState->apply();
    g_vao->drawArrays(GL_POINTS, 10, 5);

    g_normalPointSizeState->apply();
    g_vao->drawArrays(GL_POINTS, 15, 5);

    g_thickPointSizeState->apply();

    g_vao->drawArrays(GL_POINTS, 20, 1);
    g_disableRasterizerState->apply();
    g_vao->drawArrays(GL_POINTS, 21, 1);
    g_enableRasterizerState->apply();
    g_vao->drawArrays(GL_POINTS, 22, 1);
    g_disableRasterizerState->apply();
    g_vao->drawArrays(GL_POINTS, 23, 1);
    g_enableRasterizerState->apply();
    g_vao->drawArrays(GL_POINTS, 24, 1);

    g_normalPointSizeState->apply();
    g_vao->drawArrays(GL_POINTS, 25, 5);

    g_thinPointSizeState->apply();
    g_vao->drawArrays(GL_POINTS, 30, 5);

    g_thinnestPointSizeState->apply();
    g_vao->drawArrays(GL_POINTS, 35, 5);

    g_defaultPointSizeState->apply();
    g_vao->drawArrays(GL_POINTS, 35, 5);

    g_shaderProgram->release();
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
    GLFWwindow * window = glfwCreateWindow(640, 480, "globjects States", NULL, NULL);
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
