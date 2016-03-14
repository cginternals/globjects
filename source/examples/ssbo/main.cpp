
#include <algorithm>

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

using namespace gl;
using namespace glm;
using namespace globjects;


namespace
{

// taken from iozeug::FilePath::toPath
std::string normalizePath(const std::string & filepath)
{
    auto copy = filepath;
    std::replace( copy.begin(), copy.end(), '\\', '/');
    auto i = copy.find_last_of('/');
    if (i == copy.size()-1)
    {
        copy = copy.substr(0, copy.size()-1);
    }
    return copy;
}

}


namespace {
    bool g_toggleFS = false;
    bool g_isFS = false;

    ScreenAlignedQuad * g_quad = nullptr;
    Buffer * g_buffer = nullptr;
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
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
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

    if (!hasExtension(GLextension::GL_ARB_shader_storage_buffer_object))
    {
        critical() << "Shader storage buffer objects not supported.";

        glfwTerminate();
        exit(1);
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
    dataPath = normalizePath(dataPath);
    if (dataPath.size() > 0) dataPath = dataPath + "/";
    else                     dataPath = "data/";

    // Initialize OpenGL objects
    g_quad = new ScreenAlignedQuad(Shader::fromFile(GL_FRAGMENT_SHADER, dataPath + "ssbo/ssbo.frag"));
    g_quad->ref();

    g_quad->program()->setUniform("maximum",     10);
    g_quad->program()->setUniform("rowCount",    10);
    g_quad->program()->setUniform("columnCount", 10);

    int data[] = {
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

    g_buffer = new Buffer();
    g_buffer->ref();
    g_buffer->setData(sizeof(data), data, GL_STATIC_DRAW);

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
