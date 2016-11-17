
#include <algorithm>

#include <cpplocate/cpplocate.h>
#include <cpplocate/ModuleInfo.h>

#include <glbinding/gl/gl.h>
#include <glbinding/ContextInfo.h>
#include <glbinding/Version.h>

#include <GLFW/glfw3.h>

#include <globjects/globjects.h>
#include <globjects/NamedString.h>
#include <globjects/Shader.h>
#include <globjects/logging.h>
#include <globjects/base/File.h>

#include "ScreenAlignedQuad.h"

// example commons
#include "contextinfo.inl"
#include "datapath.inl"


using namespace gl;
using namespace globjects;


namespace
{

bool g_toggleFS = false;
bool g_isFS = false;

ScreenAlignedQuad * g_quad = nullptr;

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
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 1);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, true);

    // Create a context and, if valid, make it current
    GLFWwindow * window = glfwCreateWindow(640, 480, "globjects Shader Includes Shader", fs ? glfwGetPrimaryMonitor() : NULL, NULL);
    if (window == nullptr)
    {
        globjects::critical() << "Context creation failed. Terminate execution.";

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

    glClearColor(0.2f, 0.3f, 0.4f, 1.f);

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
    cpplocate::ModuleInfo moduleInfo = cpplocate::findModule("globjects");

    // Get data path
    std::string dataPath = moduleInfo.value("dataPath");
    dataPath = common::normalizePath(dataPath);
    if (dataPath.size() > 0) dataPath = dataPath + "/";
    else                     dataPath = "data/";

    // Initialize OpenGL objects
    NamedString::create("/color.glsl", new File(dataPath + "shaderincludes/color.glsl"));

    g_quad = new ScreenAlignedQuad(Shader::fromFile(GL_FRAGMENT_SHADER, dataPath + "shaderincludes/test.frag"));
    g_quad->ref();
}

void deinitialize()
{
    g_quad->unref();
}

void draw()
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    g_quad->draw();
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
