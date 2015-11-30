
#include <algorithm>
#include <random>

#include <glbinding/gl/gl.h>
#include <glbinding/ContextInfo.h>
#include <glbinding/Version.h>

#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>

#include <globjects/globjects.h>
#include <globjects/logging.h>
#include <globjects/Texture.h>

#include "ScreenAlignedQuad.h"


using namespace gl;
using namespace globjects;


void key_callback(GLFWwindow * window, int key, int /*scancode*/, int action, int /*modes*/)
{
    if (key == GLFW_KEY_ESCAPE && action == GLFW_RELEASE)
        glfwSetWindowShouldClose(window, true);
}

void draw(ScreenAlignedQuad * quad)
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    quad->draw();
}


int main(int /*argc*/, char * /*argv*/[])
{
    // Initialize GLFW with error callback and window hints
    glfwInit();
    glfwSetErrorCallback( [] (int /*error*/, const char * description) { puts(description); } );
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 1);
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


    glClearColor(0.2f, 0.3f, 0.4f, 1.f);

    {
        // Initialize
        static const int w(256);
        static const int h(256);

        unsigned char data[w * h * 4];

        std::random_device rd;
        std::mt19937 generator(rd());

        std::poisson_distribution<> r(0.2);

        for (int i = 0; i < w * h * 4; ++i)
            data[i] = static_cast<unsigned char>(255 - static_cast<unsigned char>(r(generator) * 255));

        ref_ptr<Texture> texture = Texture::createDefault(GL_TEXTURE_2D);
        texture->image2D(0, GL_RGBA8, w, h, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);

        ref_ptr<ScreenAlignedQuad> quad = new ScreenAlignedQuad(texture);
        quad->setSamplerUniform(0);

        // Main loop
        while (!glfwWindowShouldClose(window))
        {
            glfwPollEvents();
            draw(quad);
            glfwSwapBuffers(window);
        }

    }

    // Properly shutdown GLFW
    glfwTerminate();

    return 0;
}
