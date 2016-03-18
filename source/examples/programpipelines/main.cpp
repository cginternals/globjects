
#include <glm/glm.hpp>

#include <glbinding/gl/gl.h>
#include <glbinding/ContextInfo.h>
#include <glbinding/Version.h>

#include <GLFW/glfw3.h>

#include <globjects/globjects.h>
#include <globjects/base/File.h>
#include <globjects/logging.h>

#include <globjects/Buffer.h>
#include <globjects/Program.h>
#include <globjects/ProgramPipeline.h>
#include <globjects/Shader.h>
#include <globjects/VertexArray.h>
#include <globjects/VertexAttributeBinding.h>
#include <globjects/base/StaticStringSource.h>


using namespace gl;
using namespace glm;
using namespace globjects;

namespace 
{
    const char * vertexShaderCode = R"(
#version 140
#extension GL_ARB_explicit_attrib_location : require

layout (location = 0) in vec2 corner;

out vec4 color;

void main()
{
    gl_Position = vec4(corner * 2.0 - 1.0, 0.0, 1.0);
    color = vec4(corner, 0.0, 1.0);
}

)";
    const char * fragmentShaderCode = R"(
#version 140
#extension GL_ARB_explicit_attrib_location : require

layout (location = 0) out vec4 fragColor;

in vec4 color;

void main()
{
    fragColor = color;
}

)";

    bool g_toggleFS = false;
    bool g_isFS = false;

    Buffer * g_cornerBuffer = nullptr;
    Program * g_vertexProgram = nullptr;
    Program * g_fragmentProgram = nullptr;
    ProgramPipeline * g_programPipeline = nullptr;
    VertexArray * g_vao = nullptr;
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
    // Initialize OpenGL objects
    g_cornerBuffer = new Buffer();
    g_cornerBuffer->ref();
    g_vertexProgram = new Program();
    g_vertexProgram->ref();
    g_fragmentProgram = new Program();
    g_fragmentProgram->ref();
    g_programPipeline = new ProgramPipeline();
    g_programPipeline->ref();
    g_vao = new VertexArray();
    g_vao->ref();

    g_vertexProgram->attach(Shader::fromString(GL_VERTEX_SHADER,  vertexShaderCode));

    g_fragmentProgram->attach(Shader::fromString(GL_FRAGMENT_SHADER, fragmentShaderCode));

    g_programPipeline->useStages(g_vertexProgram, gl::GL_VERTEX_SHADER_BIT);
    g_programPipeline->useStages(g_fragmentProgram, gl::GL_FRAGMENT_SHADER_BIT);

    g_cornerBuffer->setData(std::array<vec2, 4>{ {
        vec2(0, 0), vec2(1, 0), vec2(0, 1), vec2(1, 1) } }, GL_STATIC_DRAW);

    g_vao->binding(0)->setAttribute(0);
    g_vao->binding(0)->setBuffer(g_cornerBuffer, 0, sizeof(vec2));
    g_vao->binding(0)->setFormat(2, GL_FLOAT);
    g_vao->enable(0);
}

void deinitialize()
{
    g_cornerBuffer->unref();
    g_vertexProgram->unref();
    g_fragmentProgram->unref();
    g_programPipeline->unref();
    g_vao->unref();
}

void draw()
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    g_programPipeline->use();
    g_vao->drawArrays(GL_TRIANGLE_STRIP, 0, 4);
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
