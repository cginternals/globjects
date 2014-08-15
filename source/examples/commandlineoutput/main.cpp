


#include <iostream>
#include <iomanip>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/transform.hpp>

#include <globjects-base/ref_ptr.h>
#include <globjects-base/formatString.h>

#include <globjects/globjects.h>
#include <globjects/Uniform.h>
#include <globjects/Program.h>
#include <globjects/Shader.h>
#include <globjects/Buffer.h>
#include <globjects/Query.h>
#include <globjects/FrameBufferObject.h>
#include <globjects/RenderBufferObject.h>
#include <globjects/Sampler.h>
#include <globjects/Sync.h>
#include <globjects/Texture.h>
#include <globjects/TransformFeedback.h>
#include <globjects/VertexArrayObject.h>
#include <globjects/logging.h>
#include <globjects/DebugMessage.h>

#include <globjects-window/ContextFormat.h>
#include <globjects-window/Context.h>
#include <globjects-window/Window.h>
#include <globjects-window/WindowEventHandler.h>

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

    virtual void initialize(glowindow::Window & window) override
    {
        ExampleWindowEventHandler::initialize(window);

        glo::DebugMessage::enable();

        std::cout << "globjects Objects tests" << std::endl;

        glo::ref_ptr<glo::Buffer> buffer(new glo::Buffer());
        std::cout << "glo::Buffer = "; glo::info() << buffer.get();

        std::cout << "glo::FrameBufferObject = "; glo::info() << glo::FrameBufferObject::defaultFBO();

        glo::ref_ptr<glo::Program> program(new glo::Program());
        std::cout << "glo::Program = "; glo::info() << program.get();

        glo::ref_ptr<glo::Query> query(new glo::Query());
        std::cout << "glo::Query = "; glo::info() << query.get();

        glo::ref_ptr<glo::RenderBufferObject> rbo(new glo::RenderBufferObject());
        std::cout << "glo::RenderBufferObject = "; glo::info() << rbo.get();

        glo::ref_ptr<glo::Sampler> sampler(new glo::Sampler());
        std::cout << "glo::Sampler = "; glo::info() << sampler.get();

        glo::ref_ptr<glo::Shader> shader(new glo::Shader(gl::GL_VERTEX_SHADER));
        std::cout << "glo::Shader = "; glo::info() << shader.get();

        glo::ref_ptr<glo::Texture> texture(new glo::Texture());
        std::cout << "glo::Texture = "; glo::info() << texture.get();

        glo::ref_ptr<glo::TransformFeedback> tf(new glo::TransformFeedback());
        std::cout << "glo::TransformFeedback = "; glo::info() << tf.get();

        glo::ref_ptr<glo::VertexArrayObject> vao(new glo::VertexArrayObject());
        std::cout << "glo::VertexArrayObject = "; glo::info() << vao.get();

        glo::ref_ptr<glo::Uniform<float>> uniform(new glo::Uniform<float>("Pi", 3.14f));
        std::cout << "glo::Uniform = "; glo::info() << uniform.get();
        std::cout << "glo::AbstractUniform = "; glo::info() << static_cast<glo::AbstractUniform*>(uniform.get());

        std::cout << "glbinding::Version = "; glo::info() << glo::version();

        std::vector<glo::Buffer*> buffers{new glo::Buffer(), new glo::Buffer()};
        std::cout << "std::vector<glo::Buffer*> = "; glo::info() << buffers;

        window.close();
    }
};

/** This example shows ... .
*/
int main(int /*argc*/, char* /*argv*/[])
{
    glowindow::ContextFormat format;
    format.setVersion(3, 2);
    format.setProfile(glowindow::ContextFormat::CoreProfile);

    glowindow::Window window;

    window.setEventHandler(new EventHandler());

    std::cout << "Standard types test" << std::endl;
    std::cout << "const char* \"Hello\" = "; glo::info() << "Hello";
    std::cout << "const std::string& \"Master\" = "; glo::info() << std::string("Master");
    std::cout << "bool true = "; glo::info() << true;
    std::cout << "char 'a' = "; glo::info() << 'a';
    std::cout << "int 42 = "; glo::info() << 42;
    std::cout << "float 3.14 = "; glo::info() << 3.14f;
    std::cout << "double 3.141592654 = "; glo::info() << std::setprecision(10) << 3.141592654;
    std::cout << "long double 2.71828 = "; glo::info() << 2.71828l;
    std::cout << "unsigned integer 23 = "; glo::info() << 23u;
    std::cout << "long integer 1337 = "; glo::info() << 1337l;
    std::cout << "long long integer 1234567890 = "; glo::info() << 1234567890ll;
    std::cout << "unsigned long integer 45123 = "; glo::info() << 45123ul;
    std::cout << "unsigned char 97 = "; glo::info() << 'a';
    std::cout << "void* " << &window << " = "; glo::info() << static_cast<void*>(&window);

    std::cout << std::endl;

    std::cout << "std::array<int, 2> = "; glo::info() << std::array<int, 2>{ { 0, 1 } };
    std::cout << "std::vector<float> = "; glo::info() << std::vector<float>({ 0.1f, 0.2f, 0.3f });

    std::cout << std::endl;

    std::cout << "Format string test" << std::endl;
    std::cout << "Expected: " << "This is a test: 42 pi = +3.14159E+00" << std::endl;
    glo::info("  Actual: This is a test: %; pi = %+0E10.5;", 42, 3.141592653589793);
    std::cout << "Expected: " << "a string - 255 - ______2.72" << std::endl;
    glo::info("  Actual: %; - %X; - %rf?_10.2;", "a string", 255, 2.71828182846);

    std::cout << std::endl;

    if (!window.create(format, "Command Line Output Example"))
    {
        return 1;
    }

    window.show();

    return glowindow::MainLoop::run();
}
