
#include <glbinding/gl/gl.h>

#include <glm/glm.hpp>
#include <glm/gtx/transform.hpp>

#include <globjects/logging.h>
#include <globjects/globjects.h>

#include <globjects/base/File.h>
#include <globjects/Uniform.h>
#include <globjects/Program.h>
#include <globjects/Shader.h>
#include <globjects/Buffer.h>

#include <common/Timer.h>
#include <common/Icosahedron.h>
#include <common/Camera.h>
#include <common/ContextFormat.h>
#include <common/Context.h>
#include <common/Window.h>
#include <common/WindowEventHandler.h>
#include <common/events.h>


using namespace gl;
using namespace glm;
using namespace globjects;

class EventHandler : public WindowEventHandler
{
public:
    EventHandler()
    : m_camera(vec3(0.f, 1.f, 4.f))
    {
    }

    virtual ~EventHandler()
    {
    }

    virtual void initialize(Window & window) override
    {
        WindowEventHandler::initialize(window);

        if (!hasExtension(GLextension::GL_ARB_tessellation_shader))
        {
            critical() << "Tesselation not supported.";

            window.close();
            return;
        }

        glClearColor(1.f, 1.f, 1.f, 0.f);

        m_vertexSource.reset(new File("data/tessellation/sphere.vert"));
        m_tessControlSource.reset(new File("data/tessellation/sphere.tcs"));
        m_tessEvaluationSource.reset(new File("data/tessellation/sphere.tes"));
        m_geometrySource.reset(new File("data/tessellation/sphere.geom"));
        m_fragmentSource.reset(new File("data/tessellation/sphere.frag"));
        m_phongSource.reset(new File("data/common/phong.frag"));

        m_vertexShader.reset(new Shader(GL_VERTEX_SHADER, m_vertexSource.get()));
        m_tessControlShader.reset(new Shader(GL_TESS_CONTROL_SHADER, m_tessControlSource.get()));
        m_tessEvaluationShader.reset(new Shader(GL_TESS_EVALUATION_SHADER, m_tessEvaluationSource.get()));
        m_geometryShader.reset(new Shader(GL_GEOMETRY_SHADER, m_geometrySource.get()));
        m_fragmentShader.reset(new Shader(GL_FRAGMENT_SHADER, m_fragmentSource.get()));
        m_phongShader.reset(new Shader(GL_FRAGMENT_SHADER, m_phongSource.get()));

        m_sphere.reset(new Program());
        m_sphere->attach(m_vertexShader.get(), m_tessControlShader.get(), m_tessEvaluationShader.get(), m_geometryShader.get(), m_phongShader.get(), m_fragmentShader.get());

        m_icosahedron.reset(new Icosahedron());

        m_time.reset();
        m_time.start();

        m_camera.setZNear(1.f);
        m_camera.setZFar(16.f);

        m_viewProjectionUniform.reset(new Uniform<glm::mat4>("transform", m_camera.viewProjection()));
        m_rotationUniform.reset(new Uniform<glm::mat4>("rotation", glm::mat4()));
        m_levelUniform.reset(new Uniform<int>("level", 0));

        m_sphere->attach(
            m_viewProjectionUniform.get(),
            m_rotationUniform.get(),
            m_levelUniform.get()
        );
    }

    virtual void framebufferResizeEvent(ResizeEvent & event) override
    {
        WindowEventHandler::framebufferResizeEvent(event);

        m_camera.setViewport(event.width(), event.height());
    }

    virtual void paintEvent(PaintEvent & event) override
    {
        WindowEventHandler::paintEvent(event);

        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        float t = static_cast<float>(m_time.elapsed().count()) * 4e-10f;
        mat4 R = rotate(t * 10.f, vec3(sin(t * 0.321f), cos(t * 0.234f), sin(t * 0.123f)));

        m_viewProjectionUniform->set(m_camera.viewProjection());
        m_rotationUniform->set(R);

        int level = static_cast<int>((sin(t) * 0.5f + 0.5f) * 16) + 1;

        m_levelUniform->set(level);

        m_sphere->use();

        glPatchParameteri(GL_PATCH_VERTICES, 3);
        m_icosahedron->draw(GL_PATCHES);

        m_sphere->release();
    }

protected:
    std::unique_ptr<Program> m_sphere;

    std::unique_ptr<AbstractStringSource> m_vertexSource;
    std::unique_ptr<AbstractStringSource> m_tessControlSource;
    std::unique_ptr<AbstractStringSource> m_tessEvaluationSource;
    std::unique_ptr<AbstractStringSource> m_geometrySource;
    std::unique_ptr<AbstractStringSource> m_fragmentSource;
    std::unique_ptr<AbstractStringSource> m_phongSource;

    std::unique_ptr<Shader> m_vertexShader;
    std::unique_ptr<Shader> m_tessControlShader;
    std::unique_ptr<Shader> m_tessEvaluationShader;
    std::unique_ptr<Shader> m_geometryShader;
    std::unique_ptr<Shader> m_fragmentShader;
    std::unique_ptr<Shader> m_phongShader;

    std::unique_ptr<Icosahedron> m_icosahedron;

    std::unique_ptr<Uniform<glm::mat4>> m_viewProjectionUniform;
    std::unique_ptr<Uniform<glm::mat4>> m_rotationUniform;
    std::unique_ptr<Uniform<int>> m_levelUniform;

    Camera m_camera;
    Timer m_time;

    vec3 m_rand;
};


int main(int /*argc*/, char * /*argv*/[])
{
    info() << "Usage:";
    info() << "\t" << "ESC" << "\t\t"       << "Close example";
    info() << "\t" << "ALT + Enter" << "\t" << "Toggle fullscreen";
    info() << "\t" << "F11" << "\t\t"       << "Toggle fullscreen";
    info() << "\t" << "F10" << "\t\t"       << "Toggle vertical sync";
    info() << "\t" << "F5" << "\t\t"        << "Reload shaders";

    ContextFormat format;
    format.setVersion(4, 0);
    format.setProfile(ContextFormat::Profile::Core);
    format.setDepthBufferSize(16);
    format.setForwardCompatible(true);

    Window::init();

    Window window;
    window.setEventHandler(new EventHandler());

    if (!window.create(format, "Tesselation Example"))
        return 1;

    window.show();

    return MainLoop::run();
}
