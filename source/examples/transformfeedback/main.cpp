
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
    std::unique_ptr<globjects::VertexArray> g_vao = nullptr;
    std::unique_ptr<globjects::Program> g_transformFeedbackProgram = nullptr;
    std::unique_ptr<globjects::TransformFeedback> g_transformFeedback = nullptr;
    std::unique_ptr<globjects::Program> g_shaderProgram = nullptr;

    std::unique_ptr<globjects::File> g_feedbackShaderSource = nullptr;
    std::unique_ptr<globjects::AbstractStringSource> g_feedbackShaderTemplate = nullptr;
    std::unique_ptr<globjects::Shader> g_feedbackShader = nullptr;

    std::unique_ptr<globjects::File> g_vertexShaderSource = nullptr;
    std::unique_ptr<globjects::AbstractStringSource> g_vertexShaderTemplate = nullptr;
    std::unique_ptr<globjects::Shader> g_vertexShader = nullptr;
    std::unique_ptr<globjects::File> g_fragmentShaderSource = nullptr;
    std::unique_ptr<globjects::AbstractStringSource> g_fragmentShaderTemplate = nullptr;
    std::unique_ptr<globjects::Shader> g_fragmentShader = nullptr;

    std::unique_ptr<globjects::Buffer> g_vertexBuffer1 = nullptr;
    std::unique_ptr<globjects::Buffer> g_vertexBuffer2 = nullptr;
    std::unique_ptr<globjects::Buffer> g_colorBuffer = nullptr;

    std::chrono::high_resolution_clock::time_point g_startTime;

    auto g_size = glm::ivec2{};
}

void initialize()
{
    const auto dataPath = common::retrieveDataPath("globjects", "dataPath");

    g_shaderProgram = globjects::Program::create();

    g_vertexShaderSource = globjects::Shader::sourceFromFile(dataPath + "transformfeedback/simple.vert");
    g_vertexShaderTemplate = globjects::Shader::applyGlobalReplacements(g_vertexShaderSource.get());
    g_vertexShader = globjects::Shader::create(GL_VERTEX_SHADER, g_vertexShaderTemplate.get());

    g_fragmentShaderSource = globjects::Shader::sourceFromFile(dataPath + "transformfeedback/simple.frag");
    g_fragmentShaderTemplate = globjects::Shader::applyGlobalReplacements(g_fragmentShaderSource.get());
    g_fragmentShader = globjects::Shader::create(GL_FRAGMENT_SHADER, g_fragmentShaderTemplate.get());

    g_shaderProgram->attach(g_vertexShader.get(), g_fragmentShader.get());

    g_transformFeedbackProgram = globjects::Program::create();

    g_feedbackShaderSource = globjects::Shader::sourceFromFile(dataPath + "transformfeedback/transformfeedback.vert");
    g_feedbackShaderTemplate = globjects::Shader::applyGlobalReplacements(g_feedbackShaderSource.get());
    g_feedbackShader = globjects::Shader::create(GL_VERTEX_SHADER, g_feedbackShaderTemplate.get());

    g_transformFeedbackProgram->attach(g_feedbackShader.get());

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

    g_vertexBuffer1 = globjects::Buffer::create();
    g_vertexBuffer1->setData(vertexArray, GL_STATIC_DRAW);
    g_vertexBuffer2 = globjects::Buffer::create();
    g_vertexBuffer2->setData(vertexArray, GL_STATIC_DRAW);
    g_colorBuffer = globjects::Buffer::create();
    g_colorBuffer->setData(colorArray, GL_STATIC_DRAW);

    g_vao = globjects::VertexArray::create();

    g_vao->binding(0)->setAttribute(0);
    g_vao->binding(0)->setFormat(4, GL_FLOAT);

    g_vao->binding(1)->setAttribute(1);
    g_vao->binding(1)->setBuffer(g_colorBuffer.get(), 0, sizeof(glm::vec4));
    g_vao->binding(1)->setFormat(4, GL_FLOAT);

    g_vao->enable(0);
    g_vao->enable(1);


    // Create and setup TransformFeedback
    g_transformFeedback = globjects::TransformFeedback::create();
    g_transformFeedback->setVaryings(g_transformFeedbackProgram.get(), { { "next_position" } }, GL_INTERLEAVED_ATTRIBS);


    g_startTime = std::chrono::high_resolution_clock::now();
}

void deinitialize()
{
    g_vao.reset(nullptr);

    g_transformFeedbackProgram.reset(nullptr);
    g_transformFeedback.reset(nullptr);
    g_shaderProgram.reset(nullptr);

    g_feedbackShaderSource.reset(nullptr);
    g_feedbackShaderTemplate.reset(nullptr);
    g_feedbackShader.reset(nullptr);

    g_vertexShaderSource.reset(nullptr);
    g_vertexShaderTemplate.reset(nullptr);
    g_vertexShader.reset(nullptr);
    g_fragmentShaderSource.reset(nullptr);
    g_fragmentShaderTemplate.reset(nullptr);
    g_fragmentShader.reset(nullptr);

    g_vertexBuffer1.reset(nullptr);
    g_vertexBuffer2.reset(nullptr);
    g_colorBuffer.reset(nullptr);
}

void draw()
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    const auto side = std::min<int>(g_size.x, g_size.y);
    glViewport((g_size.x - side) / 2, (g_size.y - side) / 2, side, side);

    const auto t_elapsed = std::chrono::high_resolution_clock::now() - g_startTime;
    g_startTime = std::chrono::high_resolution_clock::now();

    auto drawBuffer  = g_vertexBuffer1.get();
    auto writeBuffer = g_vertexBuffer2.get();

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
    {
        g_feedbackShaderSource->reload();
        g_vertexShaderSource->reload();
        g_fragmentShaderSource->reload();
    }
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
