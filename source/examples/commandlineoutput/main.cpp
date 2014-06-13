


#include <iostream>
#include <iomanip>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/transform.hpp>

#include <glowbase/ref_ptr.h>
#include <glowbase/formatString.h>

#include <glow/glow.h>
#include <glow/Uniform.h>
#include <glow/Program.h>
#include <glow/Shader.h>
#include <glow/Buffer.h>
#include <glow/Query.h>
#include <glow/FrameBufferObject.h>
#include <glow/RenderBufferObject.h>
#include <glow/Sampler.h>
#include <glow/Sync.h>
#include <glow/Texture.h>
#include <glow/TransformFeedback.h>
#include <glow/VertexArrayObject.h>
#include <glow/logging.h>
#include <glow/debugmessageoutput.h>

#include <glowwindow/ContextFormat.h>
#include <glowwindow/Context.h>
#include <glowwindow/Window.h>
#include <glowwindow/WindowEventHandler.h>

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

    virtual void initialize(glowwindow::Window & window) override
    {
        ExampleWindowEventHandler::initialize(window);

        glow::debugmessageoutput::enable();

        std::cout << "glow Objects tests" << std::endl;

        glow::ref_ptr<glow::Buffer> buffer(new glow::Buffer());
        std::cout << "glow::Buffer = "; glow::info() << buffer.get();

        std::cout << "glow::FrameBufferObject = "; glow::info() << glow::FrameBufferObject::defaultFBO();

        glow::ref_ptr<glow::Program> program(new glow::Program());
        std::cout << "glow::Program = "; glow::info() << program.get();

        glow::ref_ptr<glow::Query> query(new glow::Query());
        std::cout << "glow::Query = "; glow::info() << query.get();

        glow::ref_ptr<glow::RenderBufferObject> rbo(new glow::RenderBufferObject());
        std::cout << "glow::RenderBufferObject = "; glow::info() << rbo.get();

        glow::ref_ptr<glow::Sampler> sampler(new glow::Sampler());
        std::cout << "glow::Sampler = "; glow::info() << sampler.get();

        glow::ref_ptr<glow::Shader> shader(new glow::Shader(gl::VERTEX_SHADER));
        std::cout << "glow::Shader = "; glow::info() << shader.get();

        glow::ref_ptr<glow::Texture> texture(new glow::Texture());
        std::cout << "glow::Texture = "; glow::info() << texture.get();

        glow::ref_ptr<glow::TransformFeedback> tf(new glow::TransformFeedback());
        std::cout << "glow::TransformFeedback = "; glow::info() << tf.get();

        glow::ref_ptr<glow::VertexArrayObject> vao(new glow::VertexArrayObject());
        std::cout << "glow::VertexArrayObject = "; glow::info() << vao.get();

        glow::ref_ptr<glow::Uniform<float>> uniform(new glow::Uniform<float>("Pi", 3.14f));
        std::cout << "glow::Uniform = "; glow::info() << uniform.get();
        std::cout << "glow::AbstractUniform = "; glow::info() << static_cast<glow::AbstractUniform*>(uniform.get());

        std::cout << "glow::Version = "; glow::info() << glow::version();

        std::vector<glow::Buffer*> buffers{new glow::Buffer(), new glow::Buffer()};
        std::cout << "std::vector<glow::Buffer*> = "; glow::info() << buffers;

        window.close();
    }
};

/** This example shows ... .
*/
int main(int /*argc*/, char* /*argv*/[])
{
    glowwindow::ContextFormat format;
    format.setVersion(3, 2);
    format.setProfile(glowwindow::ContextFormat::CoreProfile);

    glowwindow::Window window;

    window.setEventHandler(new EventHandler());

    std::cout << "Standard types test" << std::endl;
    std::cout << "const char* \"Hello\" = "; glow::info() << "Hello";
    std::cout << "const std::string& \"Master\" = "; glow::info() << std::string("Master");
    std::cout << "bool true = "; glow::info() << true;
    std::cout << "char 'a' = "; glow::info() << 'a';
    std::cout << "int 42 = "; glow::info() << 42;
    std::cout << "float 3.14 = "; glow::info() << 3.14f;
    std::cout << "double 3.141592654 = "; glow::info() << std::setprecision(10) << 3.141592654;
    std::cout << "long double 2.71828 = "; glow::info() << 2.71828l;
    std::cout << "unsigned integer 23 = "; glow::info() << 23u;
    std::cout << "long integer 1337 = "; glow::info() << 1337l;
    std::cout << "long long integer 1234567890 = "; glow::info() << 1234567890ll;
    std::cout << "unsigned long integer 45123 = "; glow::info() << 45123ul;
    std::cout << "unsigned char 97 = "; glow::info() << 'a';
    std::cout << "void* " << &window << " = "; glow::info() << static_cast<void*>(&window);

    std::cout << std::endl;

    std::cout << "std::array<int, 2> = "; glow::info() << std::array<int, 2>{ { 0, 1 } };
    std::cout << "std::vector<float> = "; glow::info() << std::vector<float>({ 0.1f, 0.2f, 0.3f });

    std::cout << std::endl;

    std::cout << "Format string test" << std::endl;
    std::cout << "Expected: " << "This is a test: 42 pi = +3.14159E+00" << std::endl;
    glow::info("  Actual: This is a test: %; pi = %+0E10.5;", 42, 3.141592653589793);
    std::cout << "Expected: " << "a string - 255 - ______2.72" << std::endl;
    glow::info("  Actual: %; - %X; - %rf?_10.2;", "a string", 255, 2.71828182846);

    std::cout << std::endl;

    if (!window.create(format, "Command Line Output Example"))
    {
        return 1;
    }

    window.show();

    return glowwindow::MainLoop::run();
}
