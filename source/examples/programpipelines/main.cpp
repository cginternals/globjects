
#include <iostream>

#include <glm/vec2.hpp>

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


namespace 
{
    const auto vertexShaderCode = R"(
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
    const auto fragmentShaderCode = R"(
#version 140
#extension GL_ARB_explicit_attrib_location : require

layout (location = 0) out vec4 fragColor;

in vec4 color;

void main()
{
    fragColor = color;
}

)";

    std::unique_ptr<globjects::Buffer> g_cornerBuffer = nullptr;
    std::unique_ptr<globjects::Program> g_vertexProgram = nullptr;
    std::unique_ptr<globjects::Program> g_fragmentProgram = nullptr;
    std::unique_ptr<globjects::AbstractStringSource> g_vertexShaderSource = nullptr;
    std::unique_ptr<globjects::AbstractStringSource> g_vertexShaderTemplate = nullptr;
    std::unique_ptr<globjects::Shader> g_vertexShader = nullptr;
    std::unique_ptr<globjects::AbstractStringSource> g_fragmentShaderSource = nullptr;
    std::unique_ptr<globjects::AbstractStringSource> g_fragmentShaderTemplate = nullptr;
    std::unique_ptr<globjects::Shader> g_fragmentShader = nullptr;
    std::unique_ptr<globjects::ProgramPipeline> g_programPipeline = nullptr;
    std::unique_ptr<globjects::VertexArray> g_vao = nullptr;

    auto g_size = glm::ivec2{ };
}


void initialize()
{
    g_cornerBuffer = globjects::Buffer::create();
    g_vertexProgram = globjects::Program::create();
    g_fragmentProgram = globjects::Program::create();
    g_programPipeline = globjects::ProgramPipeline::create();
    g_vao = globjects::VertexArray::create();

    g_vertexShaderSource = globjects::Shader::sourceFromString(vertexShaderCode);
    g_vertexShaderTemplate = globjects::Shader::applyGlobalReplacements(g_vertexShaderSource.get());

    g_fragmentShaderSource = globjects::Shader::sourceFromString(fragmentShaderCode);
    g_fragmentShaderTemplate = globjects::Shader::applyGlobalReplacements(g_fragmentShaderSource.get());

    g_vertexShader = globjects::Shader::create(GL_VERTEX_SHADER, g_vertexShaderTemplate.get());
    g_fragmentShader = globjects::Shader::create(GL_FRAGMENT_SHADER, g_fragmentShaderTemplate.get());

    g_vertexProgram->attach(g_vertexShader.get());
    g_fragmentProgram->attach(g_fragmentShader.get());

    g_programPipeline->useStages(g_vertexProgram.get(), gl::GL_VERTEX_SHADER_BIT);
    g_programPipeline->useStages(g_fragmentProgram.get(), gl::GL_FRAGMENT_SHADER_BIT);

    g_cornerBuffer->setData(std::array<glm::vec2, 4>{ {
        glm::vec2(0, 0), glm::vec2(1, 0), glm::vec2(0, 1), glm::vec2(1, 1) } }, GL_STATIC_DRAW);

    g_vao->binding(0)->setAttribute(0);
    g_vao->binding(0)->setBuffer(g_cornerBuffer.get(), 0, sizeof(glm::vec2));
    g_vao->binding(0)->setFormat(2, GL_FLOAT);
    g_vao->enable(0);
}

void deinitialize()
{
    g_cornerBuffer.reset(nullptr);
    g_vertexProgram.reset(nullptr);
    g_fragmentProgram.reset(nullptr);
    g_vertexShaderSource.reset(nullptr);
    g_vertexShaderTemplate.reset(nullptr);
    g_vertexShader.reset(nullptr);
    g_fragmentShaderSource.reset(nullptr);
    g_fragmentShaderTemplate.reset(nullptr);
    g_fragmentShader.reset(nullptr);
    g_programPipeline.reset(nullptr);
    g_vao.reset(nullptr);
}

void draw()
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glViewport(0, 0, g_size.x, g_size.y);

    g_programPipeline->use();
    g_vao->drawArrays(GL_TRIANGLE_STRIP, 0, 4);
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
}


int main(int /*argc*/, char * /*argv*/[])
{
    // Initialize GLFW
    if (!glfwInit())
        return 1;

    glfwSetErrorCallback(error);
    glfwDefaultWindowHints();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 1);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, true);

    // Create a context and, if valid, make it current
    GLFWwindow * window = glfwCreateWindow(640, 480, "globjects Progam Pipelines", NULL, NULL);
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
