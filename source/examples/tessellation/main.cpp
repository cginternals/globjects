
#include <chrono>

#include <glm/glm.hpp>

#include <glm/gtx/transform.hpp>

#include <glbinding/gl/gl.h>
#include <glbinding/ContextInfo.h>
#include <glbinding/Version.h>

#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>

#include <globjects/globjects.h>
#include <globjects/logging.h>
#include <globjects/Uniform.h>
#include <globjects/Program.h>
#include <globjects/Shader.h>

#include <common/Icosahedron.h>


using namespace gl;
using namespace glm;
using namespace globjects;


void key_callback(GLFWwindow * window, int key, int /*scancode*/, int action, int /*modes*/)
{
    if (key == GLFW_KEY_ESCAPE && action == GLFW_RELEASE)
        glfwSetWindowShouldClose(window, true);
}

void draw(Program * sphere, Icosahedron * icosahedron, const std::chrono::high_resolution_clock::time_point & t_start, glm::mat4 viewProjection)
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    const auto t_elapsed = std::chrono::high_resolution_clock::now() - t_start;  
    float t = static_cast<float>(t_elapsed.count()) * 4e-10f;
    mat4 R = rotate(t * 10.f, vec3(sin(t * 0.321f), cos(t * 0.234f), sin(t * 0.123f)));

    sphere->setUniform("transform", viewProjection);
    sphere->setUniform("rotation", R);

    int level = static_cast<int>((sin(t) * 0.5f + 0.5f) * 16) + 1;
    sphere->setUniform("level", level);
    sphere->use();

    glPatchParameteri(GL_PATCH_VERTICES, 3);
    icosahedron->draw(GL_PATCHES);

    sphere->release();
}


int main(int /*argc*/, char * /*argv*/[])
{
    // Initialize GLFW with error callback and needed OpenGL version window hint
    glfwInit();
    glfwSetErrorCallback( [] (int /*error*/, const char * description) { puts(description); } );
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 0);

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

    if (!hasExtension(GLextension::GL_ARB_tessellation_shader))
    {
        critical() << "Tesselation not supported.";

        glfwTerminate();
        return 1;
    }

    glClearColor(1.f, 1.f, 1.f, 0.f);

    {
        // Initialize
        ref_ptr<Program> sphere = new Program();
        sphere->attach(
            Shader::fromFile(GL_VERTEX_SHADER,          "data/tessellation/sphere.vert")
        ,   Shader::fromFile(GL_TESS_CONTROL_SHADER,    "data/tessellation/sphere.tcs")
        ,   Shader::fromFile(GL_TESS_EVALUATION_SHADER, "data/tessellation/sphere.tes")
        ,   Shader::fromFile(GL_GEOMETRY_SHADER,        "data/tessellation/sphere.geom")
        ,   Shader::fromFile(GL_FRAGMENT_SHADER,        "data/tessellation/sphere.frag")
        ,   Shader::fromFile(GL_FRAGMENT_SHADER,        "data/common/phong.frag"));


        float fovy = radians(40.f);
        float aspect = static_cast<float>(1024) / max(static_cast<float>(768), 1.f);
        float zNear = 1.f;
        float zFar = 16.f;
        vec3 eye(0.f, 1.f, 4.f);
        vec3 center(0.0, 0.0, 0.0);
        vec3 up(0.0, 1.0, 0.0);
        
        ref_ptr<Icosahedron> icosahedron = new Icosahedron();
        const auto t_start = std::chrono::high_resolution_clock::now();
        mat4 viewProjection = perspective(fovy, aspect, zNear, zFar) * lookAt(eye, center, up);


        // Main loop
        while (!glfwWindowShouldClose(window))
        {
            glfwPollEvents();
            draw(sphere, icosahedron, t_start, viewProjection);
            glfwSwapBuffers(window);
        }

    }

    // Properly shutdown GLFW
    glfwTerminate();

    return 0;
}
