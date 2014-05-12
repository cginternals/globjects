
#include <glbinding/functions.h>

#include <algorithm>
#include <random>
#include <cassert>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/transform.hpp>

#include <glow/Error.h>
#include <glow/Uniform.h>
#include <glow/Program.h>
#include <glow/Shader.h>
#include <glow/Buffer.h>
#include <glow/logging.h>
#include <glow/debugmessageoutput.h>

#include <glowutils/Timer.h>
#include <glowutils/Icosahedron.h>
#include <glowutils/AdaptiveGrid.h>
#include <glowutils/Camera.h>
#include <glowutils/File.h>
#include <glowutils/File.h>
#include <glowutils/glowutils.h>

#include <glowwindow/ContextFormat.h>
#include <glowwindow/Context.h>
#include <glowwindow/Window.h>
#include <glowwindow/WindowEventHandler.h>
#include <glowwindow/events.h>

#include <ExampleWindowEventHandler.h>

using namespace glowwindow;
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

        glow::debugmessageoutput::enable();

        gl::ClearColor(1.0f, 1.0f, 1.0f, 0.f);


        m_sphere = new glow::Program();
        m_sphere->attach(
            glowutils::createShaderFromFile(gl::VERTEX_SHADER, "data/tessellation/sphere.vert")
        ,   glowutils::createShaderFromFile(gl::TESS_CONTROL_SHADER, "data/tessellation/sphere.tcs")
        ,   glowutils::createShaderFromFile(gl::TESS_EVALUATION_SHADER, "data/tessellation/sphere.tes")
        ,   glowutils::createShaderFromFile(gl::GEOMETRY_SHADER, "data/tessellation/sphere.geom")
        ,   glowutils::createShaderFromFile(gl::FRAGMENT_SHADER, "data/tessellation/sphere.frag")
        ,   glowutils::createShaderFromFile(gl::FRAGMENT_SHADER, "data/common/phong.frag"));

        m_icosahedron = new glowutils::Icosahedron();
        m_agrid = new glowutils::AdaptiveGrid(16U);

        m_time.reset();
        m_time.start();

        m_camera.setZNear(0.1f);
        m_camera.setZFar(16.f);

        m_agrid->setCamera(&m_camera);
    }

    virtual void framebufferResizeEvent(ResizeEvent & event) override
    {
        int width = event.width();
        int height = event.height();

        gl::Viewport(0, 0, width, height);


        m_camera.setViewport(width, height);
    }

    virtual void paintEvent(PaintEvent &) override
    {
        gl::Clear(gl::COLOR_BUFFER_BIT | gl::DEPTH_BUFFER_BIT);


        m_agrid->update();

        float t = static_cast<float>(m_time.elapsed().count()) * 4e-10f;
        mat4 R = glm::rotate(t * 10.f, vec3(sin(t * 0.321), cos(t * 0.234), sin(t * 0.123)));

        m_sphere->setUniform("transform", m_camera.viewProjection());
        m_sphere->setUniform("rotation", R);

        int level = int((sin(t) * 0.5 + 0.5) * 16) + 1;
        m_sphere->setUniform("level", level);

        m_sphere->use();
        gl::PatchParameteri(gl::PATCH_VERTICES, 3);
        m_icosahedron->draw(gl::PATCHES);
        m_sphere->release();

        m_agrid->draw();
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
            glowutils::File::reloadAll();
            break;
        }
    }

protected:
    glow::ref_ptr<glow::Program> m_sphere;

    glow::ref_ptr<glowutils::Icosahedron> m_icosahedron;
    glow::ref_ptr<glowutils::AdaptiveGrid> m_agrid;

    glowutils::Camera m_camera;
    glowutils::Timer m_time;

    vec3 m_rand;
};


/** This example shows ... .
*/
int main(int /*argc*/, char* /*argv*/[])
{
    glow::info() << "Usage:";
    glow::info() << "\t" << "ESC" << "\t\t" << "Close example";
    glow::info() << "\t" << "ALT + Enter" << "\t" << "Toggle fullscreen";
    glow::info() << "\t" << "F11" << "\t\t" << "Toggle fullscreen";
    glow::info() << "\t" << "F5" << "\t\t" << "Reload shaders";

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
