
#include <chrono>

#include <glm/glm.hpp>

#include <glm/gtx/transform.hpp>

#include <glbinding/gl/gl.h>
#include <glbinding/ContextInfo.h>
#include <glbinding/Version.h>

#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>

#include <globjects/globjects.h>
#include <globjects/base/File.h>
#include <globjects/logging.h>
#include <globjects/Uniform.h>
#include <globjects/Program.h>
#include <globjects/Shader.h>

#include "Icosahedron.h"


using namespace gl;
using namespace glm;
using namespace globjects;


namespace {
    bool g_toggleFS = false;
    bool g_isFS = false;
    
    Program * g_sphere = nullptr;
    Icosahedron * g_icosahedron = nullptr;
    glm::mat4 g_viewProjection;

    const std::chrono::high_resolution_clock::time_point g_starttime = std::chrono::high_resolution_clock::now();
}


void key_callback(GLFWwindow * window, int key, int /*scancode*/, int action, int /*modes*/)
{
    if (key == GLFW_KEY_ESCAPE && action == GLFW_RELEASE)
        glfwSetWindowShouldClose(window, true);

    if (key == GLFW_KEY_F5 && action == GLFW_RELEASE)
        File::reloadAll();

    if (key == GLFW_KEY_F11 && action == GLFW_RELEASE)
        g_toggleFS = true;
}

GLFWwindow * createWindow(bool fs = false)
{
    // Set GLFW window hints
    glfwSetErrorCallback( [] (int /*error*/, const char * description) { puts(description); } );
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 0);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, true);

    // Create a context and, if valid, make it current
    GLFWwindow * window = glfwCreateWindow(1024, 768, "", fs ? glfwGetPrimaryMonitor() : NULL, NULL);
    if (window == nullptr)
    {
        critical() << "Context creation failed. Terminate execution.";

        glfwTerminate();
        exit(1);
    }
    glfwMakeContextCurrent(window);

    // Create callback that when user presses ESC, the context should be destroyed and window closed
    glfwSetKeyCallback(window, key_callback);

    // Initialize globjects (internally initializes glbinding, and registers the current context)
    globjects::init();

    // Do only on startup
    if (!g_toggleFS)
    {
       // Dump information about context and graphics card
       info() << std::endl
           << "OpenGL Version:  " << glbinding::ContextInfo::version() << std::endl
           << "OpenGL Vendor:   " << glbinding::ContextInfo::vendor() << std::endl
           << "OpenGL Renderer: " << glbinding::ContextInfo::renderer() << std::endl;
    }

    if (!hasExtension(GLextension::GL_ARB_tessellation_shader))
    {
        critical() << "Tesselation not supported.";

        glfwTerminate();
        exit(1);
    }

    glClearColor(1.f, 1.f, 1.f, 0.f);

    g_isFS = fs;
    return window;
}

void destroyWindow(GLFWwindow * window)
{
    globjects::detachAllObjects();
    glfwDestroyWindow(window);
}

void initialize()
{
    // Initialize OpenGL objects
    g_sphere = new Program();
    g_sphere->ref();
    g_sphere->attach(
        Shader::fromFile(GL_VERTEX_SHADER,          "data/tessellation/sphere.vert")
    ,   Shader::fromFile(GL_TESS_CONTROL_SHADER,    "data/tessellation/sphere.tcs")
    ,   Shader::fromFile(GL_TESS_EVALUATION_SHADER, "data/tessellation/sphere.tes")
    ,   Shader::fromFile(GL_GEOMETRY_SHADER,        "data/tessellation/sphere.geom")
    ,   Shader::fromFile(GL_FRAGMENT_SHADER,        "data/tessellation/sphere.frag")
    ,   Shader::fromFile(GL_FRAGMENT_SHADER,        "data/common/phong.frag"));


    float fovy = radians(40.f);
    float aspect = static_cast<float>(1024) / max(static_cast<float>(768), 1.f);
    float zNear = 1.f;
    float zFar = 16.f;
    vec3 eye(0.f, 1.f, 4.f);
    vec3 center(0.0, 0.0, 0.0);
    vec3 up(0.0, 1.0, 0.0);
    
    g_icosahedron = new Icosahedron();
    g_icosahedron->ref();
    g_viewProjection = perspective(fovy, aspect, zNear, zFar) * lookAt(eye, center, up);
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
    float t = static_cast<float>(t_elapsed.count()) * 4e-10f;
    mat4 R = rotate(t * 10.f, vec3(sin(t * 0.321f), cos(t * 0.234f), sin(t * 0.123f)));

    g_sphere->setUniform("transform", g_viewProjection);
    g_sphere->setUniform("rotation", R);

    int level = static_cast<int>((sin(t) * 0.5f + 0.5f) * 16) + 1;
    g_sphere->setUniform("level", level);
    g_sphere->use();

    glPatchParameteri(GL_PATCH_VERTICES, 3);
    g_icosahedron->draw(GL_PATCHES);

    g_sphere->release();
}


int main(int /*argc*/, char * /*argv*/[])
{
    // Initialize GLFW
    glfwInit();

    GLFWwindow * window = createWindow();
    initialize();

    // Main loop
    while (!glfwWindowShouldClose(window))
    {
        glfwPollEvents();

        if (g_toggleFS)
        {
            deinitialize();
            destroyWindow(window);
            window = createWindow(!g_isFS);
            initialize();

            g_toggleFS = false;
        }

        draw();
        glfwSwapBuffers(window);
    }

    deinitialize();

    // Properly shutdown GLFW
    glfwTerminate();

    return 0;
}
