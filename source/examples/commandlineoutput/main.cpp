
#include <iostream>
#include <iomanip>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/transform.hpp>

#include <glbinding/gl/enum.h>

#include <globjects/base/ref_ptr.h>
#include <globjects/base/formatString.h>

#include <globjects/globjects.h>
#include <globjects/Uniform.h>
#include <globjects/Program.h>
#include <globjects/Shader.h>
#include <globjects/Buffer.h>
#include <globjects/Query.h>
#include <globjects/Framebuffer.h>
#include <globjects/Renderbuffer.h>
#include <globjects/Sampler.h>
#include <globjects/Sync.h>
#include <globjects/Texture.h>
#include <globjects/TransformFeedback.h>
#include <globjects/VertexArray.h>
#include <globjects/logging.h>
#include <globjects/DebugMessage.h>

#include <common/ContextFormat.h>
#include <common/Context.h>
#include <common/Window.h>
#include <common/WindowEventHandler.h>

#include <ExampleWindowEventHandler.h>

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

        std::cout << "globjects Objects tests" << std::endl;

        globjects::ref_ptr<globjects::Buffer> buffer(new globjects::Buffer());
        std::cout << "glo::Buffer = "; globjects::info() << buffer.get();

        std::cout << "glo::Framebuffer = "; globjects::info() << globjects::Framebuffer::defaultFBO();

        globjects::ref_ptr<globjects::Program> program(new globjects::Program());
        std::cout << "glo::Program = "; globjects::info() << program.get();

        globjects::ref_ptr<globjects::Query> query(new globjects::Query());
        std::cout << "glo::Query = "; globjects::info() << query.get();

        globjects::ref_ptr<globjects::Renderbuffer> rbo(new globjects::Renderbuffer());
        std::cout << "glo::Renderbuffer = "; globjects::info() << rbo.get();

        globjects::ref_ptr<globjects::Sampler> sampler(new globjects::Sampler());
        std::cout << "glo::Sampler = "; globjects::info() << sampler.get();

        globjects::ref_ptr<globjects::Shader> shader(new globjects::Shader(gl::GL_VERTEX_SHADER));
        std::cout << "glo::Shader = "; globjects::info() << shader.get();

        globjects::ref_ptr<globjects::Texture> texture(new globjects::Texture());
        std::cout << "glo::Texture = "; globjects::info() << texture.get();

        globjects::ref_ptr<globjects::TransformFeedback> tf(new globjects::TransformFeedback());
        std::cout << "glo::TransformFeedback = "; globjects::info() << tf.get();

        globjects::ref_ptr<globjects::VertexArray> vao(new globjects::VertexArray());
        std::cout << "glo::VertexArray = "; globjects::info() << vao.get();

        globjects::ref_ptr<globjects::Uniform<float>> uniform(new globjects::Uniform<float>("Pi", 3.14f));
        std::cout << "glo::Uniform = "; globjects::info() << uniform.get();
        std::cout << "glo::AbstractUniform = "; globjects::info() << static_cast<globjects::AbstractUniform*>(uniform.get());

        std::cout << "glbinding::Version = "; globjects::info() << globjects::version();

        std::vector<globjects::Buffer*> buffers{new globjects::Buffer(), new globjects::Buffer()};
        std::cout << "std::vector<glo::Buffer*> = "; globjects::info() << buffers;

        window.close();
    }
};

/** This example shows ... .
*/
int main(int /*argc*/, char* /*argv*/[])
{
    ContextFormat format;
    format.setVersion(3, 2);
    format.setProfile(ContextFormat::CoreProfile);

    Window window;

    window.setEventHandler(new EventHandler());

    std::cout << "Standard types test" << std::endl;
    std::cout << "const char* \"Hello\" = "; globjects::info() << "Hello";
    std::cout << "const std::string& \"Master\" = "; globjects::info() << std::string("Master");
    std::cout << "bool true = "; globjects::info() << true;
    std::cout << "char 'a' = "; globjects::info() << 'a';
    std::cout << "int 42 = "; globjects::info() << 42;
    std::cout << "float 3.14 = "; globjects::info() << 3.14f;
    std::cout << "double 3.141592654 = "; globjects::info() << std::setprecision(10) << 3.141592654;
    std::cout << "long double 2.71828 = "; globjects::info() << 2.71828l;
    std::cout << "unsigned integer 23 = "; globjects::info() << 23u;
    std::cout << "long integer 1337 = "; globjects::info() << 1337l;
    std::cout << "long long integer 1234567890 = "; globjects::info() << 1234567890ll;
    std::cout << "unsigned long integer 45123 = "; globjects::info() << 45123ul;
    std::cout << "unsigned char 97 = "; globjects::info() << 'a';
    std::cout << "void* " << &window << " = "; globjects::info() << static_cast<void*>(&window);

    std::cout << std::endl;

    std::cout << "std::array<int, 2> = "; globjects::info() << std::array<int, 2>{ { 0, 1 } };
    std::cout << "std::vector<float> = "; globjects::info() << std::vector<float>({ 0.1f, 0.2f, 0.3f });

    std::cout << std::endl;

    std::cout << "Format string test" << std::endl;
    std::cout << "Expected: " << "This is a test: 42 pi = +3.14159E+00" << std::endl;
    globjects::info("  Actual: This is a test: %; pi = %+0E10.5;", 42, 3.141592653589793);
    std::cout << "Expected: " << "a string - 255 - ______2.72" << std::endl;
    globjects::info("  Actual: %; - %X; - %rf?_10.2;", "a string", 255, 2.71828182846);

    std::cout << std::endl;

    if (!window.create(format, "Command Line Output Example"))
    {
        return 1;
    }

    window.show();

    return MainLoop::run();
}
