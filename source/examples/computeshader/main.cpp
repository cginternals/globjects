
#include <glm/gtc/constants.hpp>

#include <glbinding/gl/gl.h>
#include <glbinding/gl/extension.h>
#include <glbinding/gl/bitfield.h>
#include <glbinding/ContextInfo.h>
#include <glbinding/Version.h>

#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>

#include <globjects/logging.h>
#include <globjects/globjects.h>

#include <globjects/Uniform.h>
#include <globjects/Program.h>
#include <globjects/Shader.h>
#include <globjects/Buffer.h>
#include <globjects/VertexArray.h>
#include <globjects/VertexAttributeBinding.h>
#include <globjects/Texture.h>

#include "ScreenAlignedQuad.h"


using namespace gl;
using namespace glm;
using namespace globjects;


void key_callback(GLFWwindow * window, int key, int /*scancode*/, int action, int /*modes*/)
{
    if (key == GLFW_KEY_ESCAPE && action == GLFW_RELEASE)
        glfwSetWindowShouldClose(window, true);
}

void draw(Texture * texture, Program * computeProgram, ScreenAlignedQuad * quad, unsigned int & frame)
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    frame = (frame + 1) % static_cast<int>(200 * pi<double>());

    computeProgram->setUniform("roll", static_cast<float>(frame) * 0.01f);

    texture->bindActive(0);

    computeProgram->dispatchCompute(512 / 16, 512 / 16, 1); // 512^2 threads in blocks of 16^2
    computeProgram->release();

    glMemoryBarrier(GL_TEXTURE_UPDATE_BARRIER_BIT);

    quad->draw();
}

int main(int /*argc*/, char * /*argv*/[])
{
    // Initialize GLFW with error callback and window hints
    glfwInit();
    glfwSetErrorCallback( [] (int /*error*/, const char * description) { puts(description); } );
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, true);

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
    

    if (!hasExtension(GLextension::GL_ARB_compute_shader))
    {
        critical() << "Compute shaders are not supported";

        glfwTerminate();
        return 1;
    }

    glClearColor(0.2f, 0.3f, 0.4f, 1.f);

    {

        ref_ptr<Texture> texture = Texture::createDefault(GL_TEXTURE_2D);
        texture->image2D(0, GL_R32F, 512, 512, 0, GL_RED, GL_FLOAT, nullptr);
        texture->bindImageTexture(0, 0, GL_FALSE, 0, GL_WRITE_ONLY, GL_R32F);

        ref_ptr<Program> computeProgram = new Program();
        computeProgram->attach(Shader::fromFile(GL_COMPUTE_SHADER, "data/computeshader/cstest.comp"));
        computeProgram->setUniform("destTex", 0);

        ref_ptr<ScreenAlignedQuad> quad = new ScreenAlignedQuad(texture);
        quad->setSamplerUniform(0);

        unsigned int frame = 0;

        // Main loop
        while (!glfwWindowShouldClose(window))
        {
            glfwPollEvents();
            draw(texture, computeProgram, quad, frame);
            glfwSwapBuffers(window);
        }

    }

    // Properly shutdown GLFW
    glfwTerminate();

    return 0;
}
