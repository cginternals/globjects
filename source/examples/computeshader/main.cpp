
#include <iostream>
#include <algorithm>

#include <cpplocate/cpplocate.h>
#include <cpplocate/ModuleInfo.h>

#include <glm/gtc/constants.hpp>
#include <glm/vec2.hpp>

#include <glbinding/gl/gl.h>
#include <glbinding/gl/extension.h>
#include <glbinding/gl/bitfield.h>
#include <glbinding/ContextInfo.h>
#include <glbinding/Version.h>

#include <GLFW/glfw3.h>

#include <globjects/globjects.h>
#include <globjects/logging.h>
#include <globjects/base/File.h>

#include <globjects/Uniform.h>
#include <globjects/Program.h>
#include <globjects/Shader.h>
#include <globjects/Buffer.h>
#include <globjects/VertexArray.h>
#include <globjects/VertexAttributeBinding.h>
#include <globjects/Texture.h>

#include "ScreenAlignedQuad.h"
#include "datapath.inl"


using namespace gl;


namespace
{
    globjects::Texture * g_texture = nullptr;
    globjects::Program * g_computeProgram = nullptr;
    ScreenAlignedQuad * g_quad = nullptr;

    auto g_frame = 0u;
    auto g_size = glm::ivec2{ };
}


void initialize()
{
    g_texture = globjects::Texture::createDefault(GL_TEXTURE_2D);
    g_texture->image2D(0, GL_R32F, 512, 512, 0, GL_RED, GL_FLOAT, nullptr);
    g_texture->bindImageTexture(0, 0, GL_FALSE, 0, GL_WRITE_ONLY, GL_R32F);
    g_texture->setParameter(GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    g_texture->setParameter(GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    g_texture->ref();

    g_computeProgram = new globjects::Program();

    const auto dataPath = common::retrieveDataPath("globjects", "dataPath");
    g_computeProgram->attach(globjects::Shader::fromFile(GL_COMPUTE_SHADER, dataPath + "computeshader/cstest.comp"));
    g_computeProgram->setUniform("destTex", 0);
    g_computeProgram->ref();

    g_quad = new ScreenAlignedQuad(g_texture);
    g_quad->setSamplerUniform(0);
    g_quad->ref();
}

void deinitialize()
{
    g_texture->unref();
    g_computeProgram->unref();
    g_quad->unref();

    globjects::detachAllObjects();
}

void draw()
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    g_frame = (g_frame + 1) % static_cast<int>(200 * glm::pi<double>());

    g_computeProgram->setUniform("roll", static_cast<float>(g_frame) * 0.01f);

    g_texture->bindActive(0);

    g_computeProgram->dispatchCompute(512 / 16, 512 / 16, 1); // 512^2 threads in blocks of 16^2
    g_computeProgram->release();

    glMemoryBarrier(GL_TEXTURE_UPDATE_BARRIER_BIT);

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

void key_callback(GLFWwindow * window, int key, int /*scancode*/, int action, int /*mods*/)
{
    if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
        glfwSetWindowShouldClose(window, 1);

    if (key == GLFW_KEY_F5 && action == GLFW_RELEASE)
        globjects::File::reloadAll();
}


int main()
{
#ifdef SYSTEM_DARWIN
    critical() << "macOS does currently not support compute shader (OpenGL 4.3. required)."
    return 0;
#endif

    // Initialize GLFW
    if (!glfwInit())
        return 1;

    glfwSetErrorCallback(error);
    glfwDefaultWindowHints();
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, true);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    // Create a context and, if valid, make it current
    GLFWwindow * window = glfwCreateWindow(640, 480, "globjects Computer Shader", nullptr, nullptr);
    if (!window)
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

    globjects::DebugMessage::enable();


    if (!globjects::hasExtension(GLextension::GL_ARB_compute_shader))
    {
        globjects::critical() << "Compute shader not supported. Terminate execution.";

        glfwTerminate();
        return -1;
    }

    globjects::info() << "Press F5 to reload compute shader." << std::endl << std::endl;


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
