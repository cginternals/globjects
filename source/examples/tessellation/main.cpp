
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

#include <globjects-utils/Timer.h>
#include <globjects-utils/Icosahedron.h>
#include <globjects-utils/AdaptiveGrid.h>
#include <globjects-utils/Camera.h>
#include <globjects-base/File.h>
#include <globjects-base/File.h>
#include <globjects-utils/globjects-utils.h>

#include <globjects-window/ContextFormat.h>
#include <globjects-window/Context.h>
#include <globjects-window/Window.h>
#include <globjects-window/WindowEventHandler.h>
#include <globjects-window/events.h>

#include <ExampleWindowEventHandler.h>

using namespace glowindow;
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

        glo::DebugMessage::enable();

        gl::glClearColor(1.0f, 1.0f, 1.0f, 0.f);


        m_sphere = new glo::Program();
        m_sphere->attach(
            glo::Shader::fromFile(gl::GL_VERTEX_SHADER, "data/tessellation/sphere.vert")
        ,   glo::Shader::fromFile(gl::GL_TESS_CONTROL_SHADER, "data/tessellation/sphere.tcs")
        ,   glo::Shader::fromFile(gl::GL_TESS_EVALUATION_SHADER, "data/tessellation/sphere.tes")
        ,   glo::Shader::fromFile(gl::GL_GEOMETRY_SHADER, "data/tessellation/sphere.geom")
        ,   glo::Shader::fromFile(gl::GL_FRAGMENT_SHADER, "data/tessellation/sphere.frag")
        ,   glo::Shader::fromFile(gl::GL_FRAGMENT_SHADER, "data/common/phong.frag"));

        m_icosahedron = new gloutils::Icosahedron();
        m_agrid = new gloutils::AdaptiveGrid(16U);

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

        gl::glViewport(0, 0, width, height);


        m_camera.setViewport(width, height);
    }

    virtual void paintEvent(PaintEvent &) override
    {
        gl::glClear(gl::GL_COLOR_BUFFER_BIT | gl::GL_DEPTH_BUFFER_BIT);


        m_agrid->update();

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
            glo::File::reloadAll();
            break;
        }
    }

protected:
    glo::ref_ptr<glo::Program> m_sphere;

    glo::ref_ptr<gloutils::Icosahedron> m_icosahedron;
    glo::ref_ptr<gloutils::AdaptiveGrid> m_agrid;

    gloutils::Camera m_camera;
    gloutils::Timer m_time;

    vec3 m_rand;
};


/** This example shows ... .
*/
int main(int /*argc*/, char* /*argv*/[])
{
    glo::info() << "Usage:";
    glo::info() << "\t" << "ESC" << "\t\t" << "Close example";
    glo::info() << "\t" << "ALT + Enter" << "\t" << "Toggle fullscreen";
    glo::info() << "\t" << "F11" << "\t\t" << "Toggle fullscreen";
    glo::info() << "\t" << "F5" << "\t\t" << "Reload shaders";

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
