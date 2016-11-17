
#include <iostream>
#include <chrono>
#include <algorithm>

#include <cpplocate/cpplocate.h>
#include <cpplocate/ModuleInfo.h>

#include <glm/glm.hpp>

#include <glm/gtx/transform.hpp>

#include <glbinding/gl/gl.h>
#include <glbinding/ContextInfo.h>
#include <glbinding/Version.h>

#include <GLFW/glfw3.h>

#include <globjects/globjects.h>
#include <globjects/base/File.h>
#include <globjects/logging.h>
#include <globjects/Uniform.h>
#include <globjects/Program.h>
#include <globjects/Shader.h>

#include "Icosahedron.h"
#include "datapath.inl"


using namespace gl;


namespace 
{
    globjects::Program * g_sphere = nullptr;
    Icosahedron * g_icosahedron = nullptr;
    glm::mat4 g_viewProjection;

    const std::chrono::high_resolution_clock::time_point g_starttime = std::chrono::high_resolution_clock::now();

    auto g_size = glm::ivec2{};
}

void resize()
{
    static const auto fovy  = glm::radians(40.f);
    static const auto zNear  = 1.f;
    static const auto zFar   = 16.f;
    static const auto eye    = glm::vec3{ 0.f, 1.f, 4.f };
    static const auto center = glm::vec3{ 0.0, 0.0, 0.0 };
    static const auto up     = glm::vec3{ 0.0, 1.0, 0.0 };

    const auto aspect = static_cast<float>(g_size.x) / glm::max(static_cast<float>(g_size.y), 1.f);

    g_viewProjection = glm::perspective(fovy, aspect, zNear, zFar) * glm::lookAt(eye, center, up);
}

void initialize()
{
    g_sphere = new globjects::Program();
    g_sphere->ref();
    const auto dataPath = common::retrieveDataPath("globjects", "dataPath");
    g_sphere->attach(
        globjects::Shader::fromFile(GL_VERTEX_SHADER,          dataPath + "tessellation/sphere.vert")
    ,   globjects::Shader::fromFile(GL_TESS_CONTROL_SHADER,    dataPath + "tessellation/sphere.tcs")
    ,   globjects::Shader::fromFile(GL_TESS_EVALUATION_SHADER, dataPath + "tessellation/sphere.tes")
    ,   globjects::Shader::fromFile(GL_GEOMETRY_SHADER,        dataPath + "tessellation/sphere.geom")
    ,   globjects::Shader::fromFile(GL_FRAGMENT_SHADER,        dataPath + "tessellation/sphere.frag")
    ,   globjects::Shader::fromFile(GL_FRAGMENT_SHADER,        dataPath + "tessellation/phong.frag"));

    g_icosahedron = new Icosahedron();
    g_icosahedron->ref();

    resize();
}

void deinitialize()
{
    g_sphere->unref();
    g_icosahedron->unref();
}

void draw()
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    const auto t_elapsed = std::chrono::high_resolution_clock::now() - g_starttime;  
    const auto t = static_cast<float>(t_elapsed.count()) * 4e-10f;
    glm::mat4 R = glm::rotate(t * 1.f, glm::vec3(sin(t * 0.321f), cos(t * 0.234f), sin(t * 0.123f)));

    g_sphere->setUniform("transform", g_viewProjection);
    g_sphere->setUniform("rotation", R);

    const auto level = static_cast<int>((sin(t) * 0.5f + 0.5f) * 16) + 1;
    g_sphere->setUniform("level", level);
    g_sphere->use();

    glViewport(0, 0, g_size.x, g_size.y);

    glPatchParameteri(GL_PATCH_VERTICES, 3);
    g_icosahedron->draw(GL_PATCHES);

    g_sphere->release();
}


void error(int errnum, const char * errmsg)
{
    globjects::critical() << errnum << ": " << errmsg << std::endl;
}

void framebuffer_size_callback(GLFWwindow * /*window*/, int width, int height)
{
    g_size = glm::ivec2{ width, height };
    resize();
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
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 0);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, true);

    // Create a context and, if valid, make it current
    GLFWwindow * window = glfwCreateWindow(640, 480, "globjects Tessellation", NULL, NULL);
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
