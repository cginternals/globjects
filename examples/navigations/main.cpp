
#include <GL/glew.h>

#include <algorithm>
#include <random>
#include <cassert>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/transform.hpp>

#include <glow/Error.h>
#include <glow/Uniform.h>
#include <glow/ShaderFile.h>
#include <glow/Program.h>
#include <glow/Shader.h>
#include <glow/Buffer.h>
#include <glow/logging.h>
#include <glow/AutoTimer.h>
#include <glow/VertexArrayObject.h>

#include <glowutils/AxisAlignedBoundingBox.h>
#include <glowutils/MathMacros.h>
#include <glowutils/Icosahedron.h>
#include <glowutils/Camera.h>
#include <glowutils/AdaptiveGrid.h>
#include <glowutils/AbstractCoordinateProvider.h>
#include <glowutils/WorldInHandNavigation.h>

#include <glowwindow/ContextFormat.h>
#include <glowwindow/Context.h>
#include <glowwindow/Window.h>
#include <glowwindow/WindowEventHandler.h>

using namespace glow;
using namespace glm;


class EventHandler : public WindowEventHandler, AbstractCoordinateProvider
{
public:
    EventHandler()
    : m_camera(vec3(0.f, 1.f, 4.0f))
    {
        m_aabb.extend(vec3(-8.f, -1.f, -8.f));
        m_aabb.extend(vec3(8.f, 1.f, 8.f));

        m_nav.setCamera(&m_camera);
        m_nav.setCoordinateProvider(this);
        m_nav.setBoundaryHint(m_aabb);
    }

    virtual ~EventHandler()
    {
    }

    void createAndSetupTexture();
    void createAndSetupShaders();
	void createAndSetupGeometry();

    virtual void initialize(Window & window) override
    {
        DebugMessageOutput::enable();

        glClearColor(1.0f, 1.0f, 1.0f, 0.f);


        m_sphere = new Program();
        m_sphere->attach(
            Shader::fromFile(GL_VERTEX_SHADER,   "data/adaptive-grid/sphere.vert")
        ,   Shader::fromFile(GL_FRAGMENT_SHADER, "data/adaptive-grid/sphere.frag"));

        m_icosahedron = new Icosahedron(2);
        m_agrid = new AdaptiveGrid(16U);

        m_camera.setZNear(0.1f);
        m_camera.setZFar(1024.f);

        m_agrid->setCamera(&m_camera);
    }    

    virtual void framebufferResizeEvent(ResizeEvent & event) override
    {
        glViewport(0, 0, event.width(), event.height());
        m_camera.setViewport(event.width(), event.height());
    }

    virtual void paintEvent(PaintEvent &) override
    {
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        m_agrid->update();
        m_sphere->setUniform("transform", m_camera.viewProjection());

        m_sphere->use();
        m_icosahedron->draw();
        m_sphere->release();

        m_agrid->draw();
    }

    virtual void idle(Window & window) override
    {
        window.repaint();
    }

    virtual void keyPressEvent(KeyEvent & event)
    {
        const float d = 0.08f;

        switch (event.key())
        {
            case GLFW_KEY_F5:
                glow::ShaderFile::reloadAll();
                break;
        }
    }

    virtual void mousePressEvent(MouseEvent & event) override
    {
        switch (event.button())
        {
            case GLFW_MOUSE_BUTTON_LEFT:
                m_nav.panBegin(event.pos());
                event.accept();
                break;

            case GLFW_MOUSE_BUTTON_RIGHT:
                m_nav.rotateBegin(event.pos());
                event.accept();
                break;
        }
    }
    virtual void mouseMoveEvent(MouseEvent & event) override
    {
        switch (m_nav.mode())
        {
            case WorldInHandNavigation::PanInteraction:
                m_nav.panProcess(event.pos());
                event.accept();
                break;

            case WorldInHandNavigation::RotateInteraction:
                m_nav.rotateProcess(event.pos());
                event.accept();
        }
    }
    virtual void mouseReleaseEvent(MouseEvent & event) override
    {
        switch (event.button())
        {
            case GLFW_MOUSE_BUTTON_LEFT:
                m_nav.panEnd();
                event.accept();
                break;

            case GLFW_MOUSE_BUTTON_RIGHT:
                m_nav.rotateEnd();
                event.accept();
                break;
        }
    }

    void scrollEvent(ScrollEvent & event) override
    {
        if (WorldInHandNavigation::NoInteraction != m_nav.mode())
            return;

        m_nav.scaleAtMouse(event.pos(), -event.offset().y * 0.1f);
        event.accept();
    }

    virtual const float depthAt(const ivec2 & windowCoordinates)
    {
        return AbstractCoordinateProvider::depthAt(m_camera, GL_DEPTH_COMPONENT, windowCoordinates);
    }

    virtual const vec3 objAt(const ivec2 & windowCoordinates)
    {
        return unproject(m_camera, static_cast<GLenum>(GL_DEPTH_COMPONENT), windowCoordinates);
    }

    virtual const vec3 objAt(const ivec2 & windowCoordinates, const float depth)
    {
        return unproject(m_camera, depth, windowCoordinates);
    }

    virtual const glm::vec3 objAt(
        const ivec2 & windowCoordinates
    ,   const float depth
    ,   const mat4 & viewProjectionInverted)
    {
        return unproject(m_camera, viewProjectionInverted, depth, windowCoordinates);
    }

protected:

    ref_ptr<Program> m_sphere;

    ref_ptr<Icosahedron> m_icosahedron;
    ref_ptr<AdaptiveGrid> m_agrid;

    Camera m_camera;
    WorldInHandNavigation m_nav;

    AxisAlignedBoundingBox m_aabb;
};


/** This example shows ... .
*/
int main(int argc, char** argv)
{
    glewExperimental = GL_TRUE;

    ContextFormat format;
    format.setVersion(4, 0);
    format.setProfile(ContextFormat::CoreProfile);
    format.setDepthBufferSize(16);

    Window window;
    window.assign(new EventHandler());

    window.create(format, "Navigations Example");
    window.context()->setSwapInterval(Context::VerticalSyncronization);
    window.show();

    return MainLoop::run();
}
