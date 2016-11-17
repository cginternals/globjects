
#include <iostream>
#include <chrono>
#include <algorithm>

#include <cpplocate/cpplocate.h>
#include <cpplocate/ModuleInfo.h>

#include <glm/vec2.hpp>
#include <glm/vec4.hpp>
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
#include <globjects/Buffer.h>
#include <globjects/VertexArray.h>
#include <globjects/VertexAttributeBinding.h>
#include <globjects/TransformFeedback.h>

#include "datapath.inl"


using namespace gl;


namespace 
{
    globjects::VertexArray * g_vao = nullptr;
    globjects::Program * g_transformFeedbackProgram = nullptr;
    globjects::TransformFeedback * g_transformFeedback = nullptr;
    globjects::Program * g_shaderProgram = nullptr;
    globjects::Buffer * g_vertexBuffer1 = nullptr;
    globjects::Buffer * g_vertexBuffer2 = nullptr;
    globjects::Buffer * g_colorBuffer = nullptr;

    std::chrono::high_resolution_clock::time_point g_startTime;

    auto g_size = glm::ivec2{};
}

void initialize()
{
    g_shaderProgram = new globjects::Program();
    g_shaderProgram->ref();
    
    const auto dataPath = common::retrieveDataPath("globjects", "dataPath");
    g_shaderProgram->attach(
        globjects::Shader::fromFile(GL_VERTEX_SHADER,   dataPath + "transformfeedback/simple.vert")
      , globjects::Shader::fromFile(GL_FRAGMENT_SHADER, dataPath + "transformfeedback/simple.frag"));

    g_transformFeedbackProgram = new globjects::Program();
    g_transformFeedbackProgram->ref();
    g_transformFeedbackProgram->attach(
        globjects::Shader::fromFile(GL_VERTEX_SHADER, dataPath + "transformfeedback/transformfeedback.vert"));

    g_transformFeedbackProgram->setUniform("deltaT", 0.0f);

    g_shaderProgram->setUniform("modelView", glm::mat4());
    g_shaderProgram->setUniform("projection", glm::ortho(-0.4f, 1.4f, -0.4f, 1.4f, 0.f, 1.f));


    // Create and setup geometry
    auto vertexArray = std::vector<glm::vec4>({
        { 0, 0, 0, 1 }
      , { 1, 0, 0, 1 }
      , { 0, 1, 0, 1 }
      , { 1, 0, 0, 1 }
      , { 0, 1, 0, 1 }
      , { 1, 1, 0, 1 } });

    auto colorArray = std::vector<glm::vec4>({
        { 1, 0, 0, 1 }
      , { 1, 1, 0, 1 }
      , { 0, 0, 1, 1 }
      , { 1, 1, 0, 1 }
      , { 0, 0, 1, 1 }
      , { 0, 1, 0, 1 } });

    g_vertexBuffer1 = new globjects::Buffer();
    g_vertexBuffer1->ref();
    g_vertexBuffer1->setData(vertexArray, GL_STATIC_DRAW);
    g_vertexBuffer2 = new globjects::Buffer();
    g_vertexBuffer2->ref();
    g_vertexBuffer2->setData(vertexArray, GL_STATIC_DRAW);
    g_colorBuffer = new globjects::Buffer();
    g_colorBuffer->ref();
    g_colorBuffer->setData(colorArray, GL_STATIC_DRAW);

    g_vao = new globjects::VertexArray();
    g_vao->ref();

    g_vao->binding(0)->setAttribute(0);
    g_vao->binding(0)->setFormat(4, GL_FLOAT);

    g_vao->binding(1)->setAttribute(1);
    g_vao->binding(1)->setBuffer(g_colorBuffer, 0, sizeof(glm::vec4));
    g_vao->binding(1)->setFormat(4, GL_FLOAT);

    g_vao->enable(0);
    g_vao->enable(1);


    // Create and setup TransformFeedback
    g_transformFeedback = new globjects::TransformFeedback();
    g_transformFeedback->ref();
    g_transformFeedback->setVaryings(g_transformFeedbackProgram, { { "next_position" } }, GL_INTERLEAVED_ATTRIBS);


    g_startTime = std::chrono::high_resolution_clock::now();
}

void deinitialize()
{
    g_vao->unref();
    g_transformFeedbackProgram->unref();
    g_transformFeedback->unref();
    g_shaderProgram->unref();
    g_vertexBuffer1->unref();
    g_vertexBuffer2->unref();
    g_colorBuffer->unref();
}

void draw()
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    const auto side = std::min<int>(g_size.x, g_size.y);
    glViewport((g_size.x - side) / 2, (g_size.y - side) / 2, side, side);

    const auto t_elapsed = std::chrono::high_resolution_clock::now() - g_startTime;
    g_startTime = std::chrono::high_resolution_clock::now();

    auto drawBuffer  = g_vertexBuffer1;
    auto writeBuffer = g_vertexBuffer2;

    g_vao->bind();

    g_transformFeedbackProgram->setUniform("deltaT", static_cast<float>(t_elapsed.count()) * float(std::nano::num) / float(std::nano::den));

    g_vao->binding(0)->setBuffer(drawBuffer, 0, sizeof(glm::vec4));

    g_transformFeedback->bind();
    writeBuffer->bindBase(GL_TRANSFORM_FEEDBACK_BUFFER, 0);

    glEnable(GL_RASTERIZER_DISCARD);

    g_transformFeedbackProgram->use();
    g_transformFeedback->begin(GL_TRIANGLES);
    g_vao->drawArrays(GL_TRIANGLES, 0, 6);
    g_transformFeedback->end();
    glDisable(GL_RASTERIZER_DISCARD);

    g_transformFeedback->unbind();

    g_vao->binding(0)->setBuffer(writeBuffer, 0, sizeof(glm::vec4));

    g_shaderProgram->use();
    g_transformFeedback->draw(GL_TRIANGLE_STRIP);
    g_shaderProgram->release();

    g_vao->unbind();

    std::swap(g_vertexBuffer1, g_vertexBuffer2);
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
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 0);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, true);

    // Create a context and, if valid, make it current
    GLFWwindow * window = glfwCreateWindow(640, 480, "globjects Transform Feedback", NULL, NULL);
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
