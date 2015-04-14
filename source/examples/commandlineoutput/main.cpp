
#include <iostream>
#include <iomanip>
#include <memory>

#include <glbinding/gl/enum.h>

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

#include <common/ContextFormat.h>
#include <common/Context.h>
#include <common/Window.h>
#include <common/WindowEventHandler.h>


using namespace gl;
using namespace globjects;

class EventHandler : public WindowEventHandler
{
public:
    EventHandler()
    {
    }

    virtual ~EventHandler()
    {
    }

    virtual void initialize(Window & window) override
    {
        WindowEventHandler::initialize(window);

        std::cout << "Testing globjects objects:" << std::endl;

        std::unique_ptr<Buffer> buffer(new Buffer());
        std::cout << "  Buffer                : "; info() << buffer.get();

        std::cout << "  Framebuffer           : "; info() << Framebuffer::defaultFBO();

        std::unique_ptr<Program> program(new Program());
        std::cout << "  Program               : "; info() << program.get();

        std::unique_ptr<Query> query(new Query());
        std::cout << "  Query                 : "; info() << query.get();

        std::unique_ptr<Renderbuffer> rbo(new Renderbuffer());
        std::cout << "  Renderbuffer          : "; info() << rbo.get();

        std::unique_ptr<Sampler> sampler(new Sampler());
        std::cout << "  Sampler               : "; info() << sampler.get();

        std::unique_ptr<Shader> shader(new Shader(GL_VERTEX_SHADER));
        std::cout << "  Shader                : "; info() << shader.get();

        std::unique_ptr<Texture> texture(new Texture());
        std::cout << "  Texture               : "; info() << texture.get();

        std::unique_ptr<TransformFeedback> tf(new TransformFeedback());
        std::cout << "  TransformFeedback     : "; info() << tf.get();

        std::unique_ptr<VertexArray> vao(new VertexArray());
        std::cout << "  VertexArray           : "; info() << vao.get();

        std::unique_ptr<Uniform<float>> uniform(new Uniform<float>("Pi", 3.14f));
        std::cout << "  Uniform               : "; info() << uniform.get();
        std::cout << "  AbstractUniform       : "; info() << static_cast<AbstractUniform*>(uniform.get());

        std::cout << "  glbinding::Version               : "; info() << version();

        std::vector<Buffer*> buffers{new Buffer(), new Buffer()};
        std::cout << "  std::vector<Buffer *> : "; info() << buffers;
        std::cout << std::endl;

        window.close();
    }
};

int main(int /*argc*/, char * /*argv*/[])
{
    ContextFormat format;
    format.setVersion(3, 1);
    format.setForwardCompatible(true);

    Window::init();

    Window window;
    window.setEventHandler(new EventHandler());

    std::cout << std::endl;
    std::cout << "Testing Standard Types:" << std::endl;
    std::cout << "  void *      " << &window << " : "; info() << static_cast<void *>(&window);
    std::cout << "  bool                    true : "; info() << true;
    std::cout << "  char                     'a' : "; info() << 'a';
    std::cout << "  unsigned char            'a' : "; info() << static_cast<unsigned char>('a');
    std::cout << "  const char *         \"Hello\" : "; info() << "Hello";
    std::cout << "  const std::string & \"Master\" : "; info() << std::string("Master");
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

    std::cout << "Testing Container Types:" << std::endl;
    std::cout << "  std::array<int, 2> : "; info() << std::array<int, 2>{ { 0, 1 } };
    std::cout << "  std::vector<float> : "; info() << std::vector<float>({ 0.1f, 0.2f, 0.3f });
    std::cout << std::endl;

    std::cout << "Testing String Formating:" << std::endl;
    std::cout << "  Expected : " << "This is a test: 42 pi = +3.14159E+00" << std::endl;
    info("    Actual : This is a test: %; pi = %+0E10.5;", 42, 3.141592653589793);
    std::cout << "  Expected : " << "A string - 255 - ______2.72" << std::endl;
    info("    Actual : %; - %X; - %rf?_10.2;", "A string", 255, 2.71828182846);
    std::cout << std::endl;

    if (!window.create(format, "Command Line Output Example"))
        return 1;

    window.show();
    return MainLoop::run();
}
