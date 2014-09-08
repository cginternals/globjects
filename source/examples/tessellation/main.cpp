
#include <glbinding/gl/gl.h>

#include <algorithm>
#include <random>
#include <cassert>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/transform.hpp>

#include <globjects/Uniform.h>
#include <globjects/Program.h>
#include <globjects/Shader.h>
#include <globjects/Buffer.h>
#include <globjects/logging.h>
#include <globjects/DebugMessage.h>

#include <common/Timer.h>
#include <common/Icosahedron.h>
#include <common/Camera.h>
#include <globjects/base/File.h>
#include <globjects/base/File.h>

#include <common/ContextFormat.h>
#include <common/Context.h>
#include <common/Window.h>
#include <common/WindowEventHandler.h>
#include <common/events.h>

#include <ExampleWindowEventHandler.h>


using namespace glm;


class EventHandler : public ExampleWindowEventHandler
{
public:
    EventHandler()
    : m_camera(vec3(0.f, 1.f, 4.0f))
    {
    }

    virtual ~EventHandler()
    {
    }

    virtual void initialize(Window & window) override
    {
        ExampleWindowEventHandler::initialize(window);

        globjects::DebugMessage::enable();

        gl::glClearColor(1.0f, 1.0f, 1.0f, 0.f);


        m_sphere = new globjects::Program();
        m_sphere->attach(
            globjects::Shader::fromFile(gl::GL_VERTEX_SHADER, "data/tessellation/sphere.vert")
        ,   globjects::Shader::fromFile(gl::GL_TESS_CONTROL_SHADER, "data/tessellation/sphere.tcs")
        ,   globjects::Shader::fromFile(gl::GL_TESS_EVALUATION_SHADER, "data/tessellation/sphere.tes")
        ,   globjects::Shader::fromFile(gl::GL_GEOMETRY_SHADER, "data/tessellation/sphere.geom")
        ,   globjects::Shader::fromFile(gl::GL_FRAGMENT_SHADER, "data/tessellation/sphere.frag")
        ,   globjects::Shader::fromFile(gl::GL_FRAGMENT_SHADER, "data/common/phong.frag"));

        m_icosahedron = new Icosahedron();

        m_time.reset();
        m_time.start();

        m_camera.setZNear(0.1f);
        m_camera.setZFar(16.f);
    }

    virtual void framebufferResizeEvent(ResizeEvent & event) override
    {
        int width = event.width();
        int height = event.height();

        gl::glViewport(0, 0, width, height);


        m_camera.setViewport(width, height);
    }

    virtual void paintEvent(PaintEvent &) override
    {
        gl::glClear(gl::GL_COLOR_BUFFER_BIT | gl::GL_DEPTH_BUFFER_BIT);

        float t = static_cast<float>(m_time.elapsed().count()) * 4e-10f;
        mat4 R = glm::rotate(t * 10.f, vec3(sin(t * 0.321), cos(t * 0.234), sin(t * 0.123)));

        m_sphere->setUniform("transform", m_camera.viewProjection());
        m_sphere->setUniform("rotation", R);

        int level = int((sin(t) * 0.5 + 0.5) * 16) + 1;
        m_sphere->setUniform("level", level);

        m_sphere->use();
        gl::glPatchParameteri(gl::GL_PATCH_VERTICES, 3);
        m_icosahedron->draw(gl::GL_PATCHES);
        m_sphere->release();
    }

    virtual void idle(Window & window) override
    {
        window.repaint();
    }

    virtual void keyPressEvent(KeyEvent & event) override
    {
        switch (event.key())
        {
        case GLFW_KEY_F5:
            globjects::File::reloadAll();
            break;
        }
    }

protected:
    globjects::ref_ptr<globjects::Program> m_sphere;

    globjects::ref_ptr<Icosahedron> m_icosahedron;

    Camera m_camera;
    Timer m_time;

    vec3 m_rand;
};


/** This example shows ... .
*/
int main(int /*argc*/, char* /*argv*/[])
{
    globjects::info() << "Usage:";
    globjects::info() << "\t" << "ESC" << "\t\t" << "Close example";
    globjects::info() << "\t" << "ALT + Enter" << "\t" << "Toggle fullscreen";
    globjects::info() << "\t" << "F11" << "\t\t" << "Toggle fullscreen";
    globjects::info() << "\t" << "F5" << "\t\t" << "Reload shaders";

    ContextFormat format;
    format.setVersion(4, 0);
    format.setProfile(ContextFormat::CoreProfile);
    format.setDepthBufferSize(16);

    Window window;

    window.setEventHandler(new EventHandler());

    if (window.create(format, "Post Processing Example"))
    {
        window.context()->setSwapInterval(Context::VerticalSyncronization);

        window.show();

        return MainLoop::run();
    }
    else
    {
        return 1;
    }
}
