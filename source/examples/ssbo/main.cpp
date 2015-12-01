
#include <glbinding/gl/gl.h>
#include <glbinding/gl/extension.h>
#include <glbinding/ContextInfo.h>
#include <glbinding/Version.h>

#define GLFW_INCLUDE_NONE
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


namespace {
    bool toggleFS = false;
    bool isFS = false;
}


void key_callback(GLFWwindow * window, int key, int /*scancode*/, int action, int /*modes*/)
{
    if (key == GLFW_KEY_ESCAPE && action == GLFW_RELEASE)
        glfwSetWindowShouldClose(window, true);

    if (key == GLFW_KEY_F5 && action == GLFW_RELEASE)
        File::reloadAll();

    if (key == GLFW_KEY_F11 && action == GLFW_RELEASE)
        toggleFS = true;
}

GLFWwindow * initialize(bool fs = false)
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
    if (!toggleFS)
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

    isFS = fs;
    return window;
}

void deinitialize(GLFWwindow * window)
{
    globjects::detachAllObjects();
    glfwDestroyWindow(window);
}

void draw(ScreenAlignedQuad * quad)
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    quad->draw();
}


int main(int /*argc*/, char * /*argv*/[])
{
    // Initialize GLFW
    glfwInit();

    GLFWwindow * window = nullptr;

    do
    {
        // Deinitialize old window before fullscreen toggle
        if (window != nullptr) deinitialize(window);

        // Initialize window
        window = initialize(toggleFS ? !isFS : isFS);
        toggleFS = false;

        // Initialize OpenGL objects
        ref_ptr<ScreenAlignedQuad> quad = new ScreenAlignedQuad(Shader::fromFile(GL_FRAGMENT_SHADER, "data/ssbo/ssbo.frag"));

        quad->program()->setUniform("maximum",     10);
        quad->program()->setUniform("rowCount",    10);
        quad->program()->setUniform("columnCount", 10);

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

        ref_ptr<Buffer> buffer = new Buffer();
        buffer->setData(sizeof(data), data, GL_STATIC_DRAW);

        buffer->bindBase(GL_SHADER_STORAGE_BUFFER, 1);


        // Main loop
        while (!toggleFS && !glfwWindowShouldClose(window))
        {
            glfwPollEvents();
            draw(quad);
            glfwSwapBuffers(window);
        }

    }
    while (!glfwWindowShouldClose(window));

    // Properly shutdown GLFW
    glfwTerminate();

    return 0;
}