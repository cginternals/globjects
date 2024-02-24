
#include <iostream>
#include <chrono>
#include <algorithm>

#include <cpplocate/cpplocate.h>

#include <glm/glm.hpp>

#include <glm/gtx/transform.hpp>

#include <glbinding/gl/gl.h>
#include <glbinding/Version.h>
#include <glbinding-aux/ContextInfo.h>
#include <glbinding-aux/types_to_string.h>

#include <GLFW/glfw3.h>

#include <globjects/globjects.h>
#include <globjects/base/File.h>
#include <globjects/logging.h>
#include <globjects/Uniform.h>
#include <globjects/Program.h>
#include <globjects/Shader.h>

#include "Icosahedron.h"
#include "datapath.inl"


using namespace gl;


namespace 
{
    std::unique_ptr<globjects::Program> g_sphere = nullptr;

    std::unique_ptr<globjects::Program> g_program = nullptr;
    std::unique_ptr<globjects::File> g_vertexShaderSource = nullptr;
    std::unique_ptr<globjects::AbstractStringSource> g_vertexShaderTemplate = nullptr;
    std::unique_ptr<globjects::Shader> g_vertexShader = nullptr;
    std::unique_ptr<globjects::File> g_tessControlShaderSource = nullptr;
    std::unique_ptr<globjects::AbstractStringSource> g_tessControlShaderTemplate = nullptr;
    std::unique_ptr<globjects::Shader> g_tessControlShader = nullptr;
    std::unique_ptr<globjects::File> g_tessEvaluationShaderSource = nullptr;
    std::unique_ptr<globjects::AbstractStringSource> g_tessEvaluationShaderTemplate = nullptr;
    std::unique_ptr<globjects::Shader> g_tessEvaluationShader = nullptr;
    std::unique_ptr<globjects::File> g_geometryShaderSource = nullptr;
    std::unique_ptr<globjects::AbstractStringSource> g_geometryShaderTemplate = nullptr;
    std::unique_ptr<globjects::Shader> g_geometryShader = nullptr;
    std::unique_ptr<globjects::File> g_fragmentShaderSource = nullptr;
    std::unique_ptr<globjects::AbstractStringSource> g_fragmentShaderTemplate = nullptr;
    std::unique_ptr<globjects::Shader> g_fragmentShader = nullptr;
    std::unique_ptr<globjects::File> g_phongShaderSource = nullptr;
    std::unique_ptr<globjects::AbstractStringSource> g_phongShaderTemplate = nullptr;
    std::unique_ptr<globjects::Shader> g_phongShader = nullptr;

    std::unique_ptr<Icosahedron> g_icosahedron = nullptr;
    glm::mat4 g_viewProjection;

    const std::chrono::high_resolution_clock::time_point g_starttime = std::chrono::high_resolution_clock::now();

    auto g_size = glm::ivec2{};
}

void resize()
{
    static const auto fovy  = glm::radians(40.f);
    static const auto zNear  = 1.f;
    static const auto zFar   = 16.f;
    static const auto eye    = glm::vec3{ 0.f, 1.f, 4.f };
    static const auto center = glm::vec3{ 0.0, 0.0, 0.0 };
    static const auto up     = glm::vec3{ 0.0, 1.0, 0.0 };

    const auto aspect = static_cast<float>(g_size.x) / glm::max(static_cast<float>(g_size.y), 1.f);

    g_viewProjection = glm::perspective(fovy, aspect, zNear, zFar) * glm::lookAt(eye, center, up);
}

void initialize()
{
    const auto dataPath = common::determineDataPath();

    g_sphere = globjects::Program::create();

    g_vertexShaderSource = globjects::Shader::sourceFromFile(dataPath + "/tessellation/sphere.vert");
    g_vertexShaderTemplate = globjects::Shader::applyGlobalReplacements(g_vertexShaderSource.get());
    g_vertexShader = globjects::Shader::create(GL_VERTEX_SHADER, g_vertexShaderTemplate.get());

    g_tessControlShaderSource = globjects::Shader::sourceFromFile(dataPath + "/tessellation/sphere.tcs");
    g_tessControlShaderTemplate = globjects::Shader::applyGlobalReplacements(g_tessControlShaderSource.get());
    g_tessControlShader = globjects::Shader::create(GL_TESS_CONTROL_SHADER, g_tessControlShaderTemplate.get());

    g_tessEvaluationShaderSource = globjects::Shader::sourceFromFile(dataPath + "/tessellation/sphere.tes");
    g_tessEvaluationShaderTemplate = globjects::Shader::applyGlobalReplacements(g_tessEvaluationShaderSource.get());
    g_tessEvaluationShader = globjects::Shader::create(GL_TESS_EVALUATION_SHADER, g_tessEvaluationShaderTemplate.get());

    g_geometryShaderSource = globjects::Shader::sourceFromFile(dataPath + "/tessellation/sphere.geom");
    g_geometryShaderTemplate = globjects::Shader::applyGlobalReplacements(g_geometryShaderSource.get());
    g_geometryShader = globjects::Shader::create(GL_GEOMETRY_SHADER, g_geometryShaderTemplate.get());

    g_fragmentShaderSource = globjects::Shader::sourceFromFile(dataPath + "/tessellation/sphere.frag");
    g_fragmentShaderTemplate = globjects::Shader::applyGlobalReplacements(g_fragmentShaderSource.get());
    g_fragmentShader = globjects::Shader::create(GL_FRAGMENT_SHADER, g_fragmentShaderTemplate.get());

    g_phongShaderSource = globjects::Shader::sourceFromFile(dataPath + "/tessellation/phong.frag");
    g_phongShaderTemplate = globjects::Shader::applyGlobalReplacements(g_phongShaderSource.get());
    g_phongShader = globjects::Shader::create(GL_FRAGMENT_SHADER, g_phongShaderTemplate.get());

    g_sphere->attach(
        g_vertexShader.get(),
        g_tessControlShader.get(),
        g_tessEvaluationShader.get(),
        g_geometryShader.get(),
        g_fragmentShader.get(),
        g_phongShader.get()
    );

    g_icosahedron = Icosahedron::create();

    resize();
}

void deinitialize()
{
    g_sphere.reset(nullptr);

    g_program.reset(nullptr);
    g_vertexShader.reset(nullptr);
    g_vertexShaderTemplate.reset(nullptr);
    g_vertexShaderSource.reset(nullptr);
    g_tessControlShader.reset(nullptr);
    g_tessControlShaderTemplate.reset(nullptr);
    g_tessControlShaderSource.reset(nullptr);
    g_tessEvaluationShader.reset(nullptr);
    g_tessEvaluationShaderTemplate.reset(nullptr);
    g_tessEvaluationShaderSource.reset(nullptr);
    g_geometryShader.reset(nullptr);
    g_geometryShaderTemplate.reset(nullptr);
    g_geometryShaderSource.reset(nullptr);
    g_fragmentShader.reset(nullptr);
    g_fragmentShaderTemplate.reset(nullptr);
    g_fragmentShaderSource.reset(nullptr);
    g_phongShader.reset(nullptr);
    g_phongShaderTemplate.reset(nullptr);
    g_phongShaderSource.reset(nullptr);

    g_icosahedron.reset(nullptr);
}

void draw()
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    const auto t_elapsed = std::chrono::high_resolution_clock::now() - g_starttime;  
    const auto t = static_cast<float>(t_elapsed.count()) * 4e-10f;
    glm::mat4 R = glm::rotate(t * 1.f, glm::vec3(sin(t * 0.321f), cos(t * 0.234f), sin(t * 0.123f)));

    g_sphere->setUniform("transform", g_viewProjection);
    g_sphere->setUniform("rotation", R);

    const auto level = static_cast<int>((sin(t) * 0.5f + 0.5f) * 16) + 1;
    g_sphere->setUniform("level", level);
    g_sphere->use();

    glViewport(0, 0, g_size.x, g_size.y);

    glPatchParameteri(GL_PATCH_VERTICES, 3);
    g_icosahedron->draw(GL_PATCHES);

    g_sphere->release();
}


void error(int errnum, const char * errmsg)
{
    globjects::critical() << errnum << ": " << errmsg << std::endl;
}

void framebuffer_size_callback(GLFWwindow * /*window*/, int width, int height)
{
    g_size = glm::ivec2{ width, height };
    resize();
}

void key_callback(GLFWwindow * window, int key, int /*scancode*/, int action, int /*modes*/)
{
    if (key == GLFW_KEY_ESCAPE && action == GLFW_RELEASE)
        glfwSetWindowShouldClose(window, true);

    if (key == GLFW_KEY_F5 && action == GLFW_RELEASE)
    {
        g_vertexShaderSource->reload();
        g_tessControlShaderSource->reload();
        g_tessEvaluationShaderSource->reload();
        g_geometryShaderSource->reload();
        g_fragmentShaderSource->reload();
        g_phongShaderSource->reload();
    }
}

void glfwError(int error_code, const char *description)
{
    globjects::info() << "glfw3 (" << error_code << "): " << description << std::endl;
}


int main(int /*argc*/, char * /*argv*/[])
{
    glfwSetErrorCallback(glfwError);
    
    // Initialize GLFW
    if (!glfwInit())
    {
        globjects::critical() << "GLFW could not be initialized. Terminate execution.";

        return 1;
    }

    glfwSetErrorCallback(error);

    glfwDefaultWindowHints();
    glfwWindowHint(GLFW_VISIBLE, true);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, true);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
#ifdef SYSTEM_DARWIN
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 1);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
#else
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 0);
#endif

    // Create a context and, if valid, make it current
    GLFWwindow * window = glfwCreateWindow(640, 480, "globjects Tessellation", nullptr, nullptr);
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
    globjects::init([](const char * name) {
        return glfwGetProcAddress(name);
    });

    std::cout << std::endl
        << "OpenGL Version:  " << glbinding::aux::ContextInfo::version() << std::endl
        << "OpenGL Vendor:   " << glbinding::aux::ContextInfo::vendor() << std::endl
        << "OpenGL Renderer: " << glbinding::aux::ContextInfo::renderer() << std::endl << std::endl;

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
