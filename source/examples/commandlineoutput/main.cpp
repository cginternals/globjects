
#include <iostream>

#include <glbinding/gl/gl.h>
#include <glbinding-aux/ContextInfo.h>
#include <glbinding/Version.h>
#include <glbinding-aux/types_to_string.h>

#include <GLFW/glfw3.h>

#include <globjects/globjects.h>
#include <globjects/logging.h>

#include <globjects/Uniform.h>
#include <globjects/Program.h>
#include <globjects/Shader.h>
#include <globjects/Buffer.h>
#include <globjects/Query.h>
#include <globjects/Framebuffer.h>
#include <globjects/Renderbuffer.h>
#include <globjects/Sampler.h>
#include <globjects/Texture.h>
#include <globjects/TransformFeedback.h>
#include <globjects/VertexArray.h>


using namespace gl;
using namespace globjects;


void error(int errnum, const char * errmsg)
{
    globjects::critical() << errnum << ": " << errmsg << std::endl;
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
    glfwWindowHint(GLFW_VISIBLE, false);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, true);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
#ifdef SYSTEM_DARWIN
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 2);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
#else
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 1);
#endif

    // Create a context and, if valid, make it current
    GLFWwindow * offscreen_context = glfwCreateWindow(320, 240, "globjects Command Line Output", nullptr, nullptr);
    if (offscreen_context == nullptr)
    {
        critical() << "Context creation failed. Terminate execution.";

        glfwTerminate();
        return 1;
    }
    glfwMakeContextCurrent(offscreen_context);

    // Initialize globjects (internally initializes glbinding, and registers the current context)
    globjects::init([](const char * name) {
        return glfwGetProcAddress(name);
    });

    std::cout << std::endl
        << "OpenGL Version:  " << glbinding::aux::ContextInfo::version() << std::endl
        << "OpenGL Vendor:   " << glbinding::aux::ContextInfo::vendor() << std::endl
        << "OpenGL Renderer: " << glbinding::aux::ContextInfo::renderer() << std::endl << std::endl;

    std::cout << std::endl;
    std::cout << "Test Logging of Standard Types:" << std::endl;
    std::cout << "  void *      " << &offscreen_context << " : "; info() << static_cast<void *>(&offscreen_context);
    std::cout << "  bool                    true : "; info() << true;
    std::cout << "  char                     'a' : "; info() << 'a';
    std::cout << "  unsigned char            'a' : "; info() << static_cast<unsigned char>('a');
    std::cout << "  const char *         \"hello\" : "; info() << "hello";
    std::cout << "  const std::string &  \"world\" : "; info() << std::string("world");
    std::cout << "  short                  32767 : "; info() << 32767;
    std::cout << "  int               2147483647 : "; info() << 2147483647;
    std::cout << "  unsigned integer          23 : "; info() << 23u;
    std::cout << "  long integer            1337 : "; info() << 1337l;
    std::cout << "  long long integer 1234567890 : "; info() << 1234567890ll;
    std::cout << "  unsigned long integer  45123 : "; info() << 45123ul;
    std::cout << "  float                3.14159 : "; info() << std::setprecision( 6) << 3.141592654f;
    std::cout << "  double           3.141592654 : "; info() << std::setprecision(10) << 3.141592654;
    std::cout << "  long double          2.71828 : "; info() << 2.71828l;
    std::cout << std::endl;

    std::cout << "Test Logging of Container Types:" << std::endl;
    std::cout << "  std::array<int, 2> : "; info() << std::array<int, 2>{ { 0, 1 } };
    std::cout << "  std::vector<float> : "; info() << std::vector<float>({ 0.1f, 0.2f, 0.3f });
    std::cout << std::endl;

    std::cout << "Test Logging of globjects objects:" << std::endl;
    {
        auto buffer = Buffer::create();
        std::cout << "  Buffer                : "; info() << buffer.get();
        std::cout << "  Framebuffer           : "; info() << Framebuffer::defaultFBO().get();
        auto program = Program::create();
        std::cout << "  Program               : "; info() << program.get();
        auto query = Query::create();
        std::cout << "  Query                 : "; info() << query.get();
        auto rbo = Renderbuffer::create();
        std::cout << "  Renderbuffer          : "; info() << rbo.get();
        auto sampler = Sampler::create();
        std::cout << "  Sampler               : "; info() << sampler.get();
        auto shader = Shader::create(GL_VERTEX_SHADER);
        std::cout << "  Shader                : "; info() << shader.get();
        auto texture = Texture::create();
        std::cout << "  Texture               : "; info() << texture.get();
        auto tf = TransformFeedback::create();
        std::cout << "  TransformFeedback     : "; info() << tf.get();
        auto vao = VertexArray::create();
        std::cout << "  VertexArray           : "; info() << vao.get();
        auto uniform = Uniform<float>::create(program.get(), "Pi", 3.14f);
        std::cout << "  Uniform               : "; info() << uniform.get();
        std::cout << "  AbstractUniform       : "; info() << static_cast<AbstractUniform*>(uniform.get());
        std::cout << "  glbinding::Version    : "; info() << version();

        auto buffer1 = Buffer::create();
        auto buffer2 = Buffer::create();
        std::vector<Buffer *> buffers{ buffer1.get(), buffer2.get() };
        std::cout << "  std::vector<Buffer *> : "; info() << buffers;
        std::cout << std::endl;
    }

    // Properly shutdown GLFW
    glfwTerminate();

    return 0;
}
