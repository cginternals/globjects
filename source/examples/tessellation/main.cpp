
#include <glbinding/gl/gl.h>

#include <glm/glm.hpp>
#include <glm/gtx/transform.hpp>

#include <globjects/logging.h>

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

        glClearColor(1.f, 1.f, 1.f, 0.f);

        m_sphere = new Program();
        m_sphere->attach(
            Shader::fromFile(GL_VERTEX_SHADER,          "data/tessellation/sphere.vert")
        ,   Shader::fromFile(GL_TESS_CONTROL_SHADER,    "data/tessellation/sphere.tcs")
        ,   Shader::fromFile(GL_TESS_EVALUATION_SHADER, "data/tessellation/sphere.tes")
        ,   Shader::fromFile(GL_GEOMETRY_SHADER,        "data/tessellation/sphere.geom")
        ,   Shader::fromFile(GL_FRAGMENT_SHADER,        "data/tessellation/sphere.frag")
        ,   Shader::fromFile(GL_FRAGMENT_SHADER,        "data/common/phong.frag"));

        m_icosahedron = new Icosahedron();

        m_time.reset();
        m_time.start();

        m_camera.setZNear(1.f);
        m_camera.setZFar(16.f);
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

        m_sphere->setUniform("transform", m_camera.viewProjection());
        m_sphere->setUniform("rotation", R);

        int level = static_cast<int>((sin(t) * 0.5f + 0.5f) * 16) + 1;
        m_sphere->setUniform("level", level);
        m_sphere->use();

        glPatchParameteri(GL_PATCH_VERTICES, 3);
        m_icosahedron->draw(GL_PATCHES);

        m_sphere->release();
    }

protected:
    ref_ptr<Program> m_sphere;

    ref_ptr<Icosahedron> m_icosahedron;

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

    Window::init();

    Window window;
    window.setEventHandler(new EventHandler());

    if (!window.create(format, "Tesselation Example"))
        return 1;

    window.show();

    return MainLoop::run();
}
