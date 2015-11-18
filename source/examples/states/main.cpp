
#include <glm/glm.hpp>

#include <glbinding/gl/gl.h>
#include <glbinding/ContextInfo.h>
#include <glbinding/Version.h>

#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>

#include <globjects/globjects.h>
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


void key_callback(GLFWwindow * window, int key, int /*scancode*/, int action, int /*modes*/)
{
    if (key == GLFW_KEY_ESCAPE && action == GLFW_RELEASE)
        glfwSetWindowShouldClose(window, true);
}

void draw(Program * shaderProgram, VertexArray * vao, State * thinnestPointSizeState, State * thinPointSizeState, State * normalPointSizeState, State * thickPointSizeState, State * disableRasterizerState, State * enableRasterizerState, State * defaultPointSizeState)
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    shaderProgram->use();

    defaultPointSizeState->apply();
    vao->drawArrays(GL_POINTS, 0, 5);

    thinnestPointSizeState->apply();
    vao->drawArrays(GL_POINTS, 5, 5);

    thinPointSizeState->apply();
    vao->drawArrays(GL_POINTS, 10, 5);

    normalPointSizeState->apply();
    vao->drawArrays(GL_POINTS, 15, 5);

    thickPointSizeState->apply();

    vao->drawArrays(GL_POINTS, 20, 1);
    disableRasterizerState->apply();
    vao->drawArrays(GL_POINTS, 21, 1);
    enableRasterizerState->apply();
    vao->drawArrays(GL_POINTS, 22, 1);
    disableRasterizerState->apply();
    vao->drawArrays(GL_POINTS, 23, 1);
    enableRasterizerState->apply();
    vao->drawArrays(GL_POINTS, 24, 1);

    normalPointSizeState->apply();
    vao->drawArrays(GL_POINTS, 25, 5);

    thinPointSizeState->apply();
    vao->drawArrays(GL_POINTS, 30, 5);

    thinnestPointSizeState->apply();
    vao->drawArrays(GL_POINTS, 35, 5);

    defaultPointSizeState->apply();
    vao->drawArrays(GL_POINTS, 35, 5);

    shaderProgram->release();
}


int main(int /*argc*/, char * /*argv*/[])
{
    // Initialize GLFW with error callback and needed OpenGL version window hint
    glfwInit();
    glfwSetErrorCallback( [] (int /*error*/, const char * description) { puts(description); } );
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 1);

    // Create a context and, if valid, make it current
    GLFWwindow * window = glfwCreateWindow(1024, 768, "", NULL, NULL);
    if (window == nullptr)
    {
        critical() << "Context creation failed. Terminate execution.";

        glfwTerminate();
        return 1;
    }
    glfwMakeContextCurrent(window);

    // Create callback that when user presses ESC, the context should be destroyed and window closed
    glfwSetKeyCallback(window, key_callback);

    // Initialize globjects (internally initializes glbinding, and registers the current context)
    globjects::init();

    // Dump information about context and graphics card
    info() << std::endl
        << "OpenGL Version:  " << glbinding::ContextInfo::version() << std::endl
        << "OpenGL Vendor:   " << glbinding::ContextInfo::vendor() << std::endl
        << "OpenGL Renderer: " << glbinding::ContextInfo::renderer() << std::endl;
    

    glClearColor(0.2f, 0.3f, 0.4f, 1.f);

    {
        // Initialize
        ref_ptr<State> defaultPointSizeState  = new State();
        defaultPointSizeState->pointSize(getFloat(GL_POINT_SIZE));
        ref_ptr<State> thinnestPointSizeState = new State();
        thinnestPointSizeState->pointSize(2.0f);
        ref_ptr<State> thinPointSizeState     = new State();
        thinPointSizeState->pointSize(5.0f);
        ref_ptr<State> normalPointSizeState   = new State();
        normalPointSizeState->pointSize(10.0f);
        ref_ptr<State> thickPointSizeState    = new State();
        thickPointSizeState->pointSize(20.0f);
        ref_ptr<State> disableRasterizerState = new State();
        disableRasterizerState->enable(GL_RASTERIZER_DISCARD);
        ref_ptr<State> enableRasterizerState  = new State();
        enableRasterizerState->disable(GL_RASTERIZER_DISCARD);

        ref_ptr<VertexArray> vao = new VertexArray();
        ref_ptr<Buffer> buffer = new Buffer();

        ref_ptr<Program> shaderProgram = new Program();
        shaderProgram->attach(
            Shader::fromFile(GL_VERTEX_SHADER, "data/states/standard.vert")
          , Shader::fromFile(GL_FRAGMENT_SHADER, "data/states/standard.frag"));
        
        buffer->setData(std::vector<vec2>({
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

        vao->binding(0)->setAttribute(0);
        vao->binding(0)->setBuffer(buffer, 0, sizeof(vec2));
        vao->binding(0)->setFormat(2, GL_FLOAT);
        vao->enable(0);


        // Main loop
        while (!glfwWindowShouldClose(window))
        {
            glfwPollEvents();
            draw(shaderProgram, vao, thinnestPointSizeState, thinPointSizeState, normalPointSizeState, thickPointSizeState, disableRasterizerState, enableRasterizerState, defaultPointSizeState);
            glfwSwapBuffers(window);
        }

    }

    // Properly shutdown GLFW
    glfwTerminate();

    return 0;
}
