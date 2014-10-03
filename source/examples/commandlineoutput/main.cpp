
#include <iostream>
#include <iomanip>

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
#include <globjects/DebugMessage.h>

#include <common/ContextFormat.h>
#include <common/Context.h>
#include <common/Window.h>
#include <common/WindowEventHandler.h>

#include <ExampleWindowEventHandler.h>


using namespace gl;

class EventHandler : public ExampleWindowEventHandler
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
        ExampleWindowEventHandler::initialize(window);

        globjects::DebugMessage::enable();

        std::cout << "Testing globjects objects:" << std::endl;

        globjects::ref_ptr<globjects::Buffer> buffer(new globjects::Buffer());
        std::cout << "  globjects::Buffer                : "; globjects::info() << buffer.get();

        std::cout << "  globjects::Framebuffer           : "; globjects::info() << globjects::Framebuffer::defaultFBO();

        globjects::ref_ptr<globjects::Program> program(new globjects::Program());
        std::cout << "  globjects::Program               : "; globjects::info() << program.get();

        globjects::ref_ptr<globjects::Query> query(new globjects::Query());
        std::cout << "  globjects::Query                 : "; globjects::info() << query.get();

        globjects::ref_ptr<globjects::Renderbuffer> rbo(new globjects::Renderbuffer());
        std::cout << "  globjects::Renderbuffer          : "; globjects::info() << rbo.get();

        globjects::ref_ptr<globjects::Sampler> sampler(new globjects::Sampler());
        std::cout << "  globjects::Sampler               : "; globjects::info() << sampler.get();

        globjects::ref_ptr<globjects::Shader> shader(new globjects::Shader(GL_VERTEX_SHADER));
        std::cout << "  globjects::Shader                : "; globjects::info() << shader.get();

        globjects::ref_ptr<globjects::Texture> texture(new globjects::Texture());
        std::cout << "  globjects::Texture               : "; globjects::info() << texture.get();

        globjects::ref_ptr<globjects::TransformFeedback> tf(new globjects::TransformFeedback());
        std::cout << "  globjects::TransformFeedback     : "; globjects::info() << tf.get();

        globjects::ref_ptr<globjects::VertexArray> vao(new globjects::VertexArray());
        std::cout << "  globjects::VertexArray           : "; globjects::info() << vao.get();

        globjects::ref_ptr<globjects::Uniform<float>> uniform(new globjects::Uniform<float>("Pi", 3.14f));
        std::cout << "  globjects::Uniform               : "; globjects::info() << uniform.get();
        std::cout << "  globjects::AbstractUniform       : "; globjects::info() << static_cast<globjects::AbstractUniform*>(uniform.get());

        std::cout << "  glbinding::Version               : "; globjects::info() << globjects::version();

        std::vector<globjects::Buffer*> buffers{new globjects::Buffer(), new globjects::Buffer()};
        std::cout << "  std::vector<globjects::Buffer *> : "; globjects::info() << buffers;
        std::cout << std::endl;

        window.close();
    }
};

int main(int /*argc*/, char* /*argv*/[])
{
    ContextFormat format;
    format.setVersion(3, 2);
    format.setProfile(ContextFormat::CoreProfile);

    Window window;

    window.setEventHandler(new EventHandler());

    std::cout << std::endl;
    std::cout << "Testing Standard Types:" << std::endl;
    std::cout << "  void *      " << &window << " : "; globjects::info() << static_cast<void *>(&window);
    std::cout << "  bool                    true : "; globjects::info() << true;
    std::cout << "  char                     'a' : "; globjects::info() << 'a';
    std::cout << "  unsigned char            'a' : "; globjects::info() << static_cast<unsigned char>('a');
    std::cout << "  const char *         \"Hello\" : "; globjects::info() << "Hello";
    std::cout << "  const std::string & \"Master\" : "; globjects::info() << std::string("Master");
    std::cout << "  short                  32767 : "; globjects::info() << 32767;
    std::cout << "  int               2147483647 : "; globjects::info() << 2147483647;
    std::cout << "  unsigned integer          23 : "; globjects::info() << 23u;
    std::cout << "  long integer            1337 : "; globjects::info() << 1337l;
    std::cout << "  long long integer 1234567890 : "; globjects::info() << 1234567890ll;
    std::cout << "  unsigned long integer  45123 : "; globjects::info() << 45123ul;
    std::cout << "  float                3.14159 : "; globjects::info() << std::setprecision( 6) << 3.141592654f;
    std::cout << "  double           3.141592654 : "; globjects::info() << std::setprecision(10) << 3.141592654;
    std::cout << "  long double          2.71828 : "; globjects::info() << 2.71828l;
    std::cout << std::endl;

    std::cout << "Testing Container Types:" << std::endl;
    std::cout << "  std::array<int, 2> : "; globjects::info() << std::array<int, 2>{ { 0, 1 } };
    std::cout << "  std::vector<float> : "; globjects::info() << std::vector<float>({ 0.1f, 0.2f, 0.3f });
    std::cout << std::endl;

    std::cout << "Testing String Formating:" << std::endl;
    std::cout << "  Expected : " << "This is a test: 42 pi = +3.14159E+00" << std::endl;
    globjects::info("    Actual : This is a test: %; pi = %+0E10.5;", 42, 3.141592653589793);
    std::cout << "  Expected : " << "A string - 255 - ______2.72" << std::endl;
    globjects::info("    Actual : %; - %X; - %rf?_10.2;", "A string", 255, 2.71828182846);
    std::cout << std::endl;

    if (!window.create(format, "Command Line Output Example"))
        return 1;

    window.show();
    return MainLoop::run();
}
