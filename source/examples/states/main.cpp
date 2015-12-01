
#include <glm/glm.hpp>

#include <glbinding/gl/gl.h>
#include <glbinding/ContextInfo.h>
#include <glbinding/Version.h>

#define GLFW_INCLUDE_NONE
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


using namespace gl;
using namespace glm;
using namespace globjects;


namespace {
    bool g_toggleFS = false;
    bool g_isFS = false;

    Program * g_shaderProgram = nullptr;
    VertexArray * g_vao = nullptr;
    Buffer * g_buffer = nullptr;
    State * g_thinnestPointSizeState = nullptr;
    State * g_thinPointSizeState = nullptr;
    State * g_normalPointSizeState = nullptr;
    State * g_thickPointSizeState = nullptr;
    State * g_disableRasterizerState = nullptr;
    State * g_enableRasterizerState = nullptr;
    State * g_defaultPointSizeState = nullptr;
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
    g_defaultPointSizeState = new State();
    g_defaultPointSizeState->ref();
    g_defaultPointSizeState->pointSize(getFloat(GL_POINT_SIZE));
    g_thinnestPointSizeState = new State();
    g_thinnestPointSizeState->ref();
    g_thinnestPointSizeState->pointSize(2.0f);
    g_thinPointSizeState = new State();
    g_thinPointSizeState->ref();
    g_thinPointSizeState->pointSize(5.0f);
    g_normalPointSizeState = new State();
    g_normalPointSizeState->ref();
    g_normalPointSizeState->pointSize(10.0f);
    g_thickPointSizeState = new State();
    g_thickPointSizeState->ref();
    g_thickPointSizeState->pointSize(20.0f);
    g_disableRasterizerState = new State();
    g_disableRasterizerState->ref();
    g_disableRasterizerState->enable(GL_RASTERIZER_DISCARD);
    g_enableRasterizerState = new State();
    g_enableRasterizerState->ref();
    g_enableRasterizerState->disable(GL_RASTERIZER_DISCARD);

    g_vao = new VertexArray();
    g_vao->ref();
    g_buffer = new Buffer();
    g_buffer->ref();

    g_shaderProgram = new Program();
    g_shaderProgram->ref();
    g_shaderProgram->attach(
        Shader::fromFile(GL_VERTEX_SHADER, "data/states/standard.vert")
      , Shader::fromFile(GL_FRAGMENT_SHADER, "data/states/standard.frag"));
    
    g_buffer->setData(std::vector<vec2>({
        vec2(-0.8f, 0.8f), vec2(-0.4f, 0.8f), vec2( 0.0f, 0.8f), vec2( 0.4f, 0.8f), vec2( 0.8f, 0.8f)
      , vec2(-0.8f, 0.6f), vec2(-0.4f, 0.6f), vec2( 0.0f, 0.6f), vec2( 0.4f, 0.6f), vec2( 0.8f, 0.6f)
      , vec2(-0.8f, 0.4f), vec2(-0.4f, 0.4f), vec2( 0.0f, 0.4f), vec2( 0.4f, 0.4f), vec2( 0.8f, 0.4f)
      , vec2(-0.8f, 0.2f), vec2(-0.4f, 0.2f), vec2( 0.0f, 0.2f), vec2( 0.4f, 0.2f), vec2( 0.8f, 0.2f)
      , vec2(-0.8f, 0.0f), vec2(-0.4f, 0.0f), vec2( 0.0f, 0.0f), vec2( 0.4f, 0.0f), vec2( 0.8f, 0.0f)
      , vec2(-0.8f,-0.2f), vec2(-0.4f,-0.2f), vec2( 0.0f,-0.2f), vec2( 0.4f,-0.2f), vec2( 0.8f,-0.2f)
      , vec2(-0.8f,-0.4f), vec2(-0.4f,-0.4f), vec2( 0.0f,-0.4f), vec2( 0.4f,-0.4f), vec2( 0.8f,-0.4f)
      , vec2(-0.8f,-0.6f), vec2(-0.4f,-0.6f), vec2( 0.0f,-0.6f), vec2( 0.4f,-0.6f), vec2( 0.8f,-0.6f)
      , vec2(-0.8f,-0.8f), vec2(-0.4f,-0.8f), vec2( 0.0f,-0.8f), vec2( 0.4f,-0.8f), vec2( 0.8f,-0.8f) })
      , GL_STATIC_DRAW );

    g_vao->binding(0)->setAttribute(0);
    g_vao->binding(0)->setBuffer(g_buffer, 0, sizeof(vec2));
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
}

void draw()
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

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
