
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

    virtual void initialize(Window & window)
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

    virtual void resizeEvent(
        Window & window
    ,   unsigned int width
    ,   unsigned int height)
    {
        glViewport(0, 0, width, height);
        m_camera.setViewport(width, height);
    }

    virtual void paintEvent(Window & window)
    {
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        m_agrid->update();
        m_sphere->setUniform("transform", m_camera.viewProjection());

        m_sphere->use();
        m_icosahedron->draw();
        m_sphere->release();

        m_agrid->draw();
    }

    virtual void idleEvent(Window & window)
    {
        window.repaint();
    }

    virtual void keyPressEvent(
        Window & window
    ,   KeyEvent & event)
    {
        const float d = 0.08f;

        switch (event.key())
        {
            case GLFW_KEY_F5:
                glow::ShaderFile::reloadAll();
                break;
        }
    }

    virtual void mousePressEvent(
        Window & window
    ,   MouseEvent & event)
    {
        switch (event.button())
        {
        case GLFW_MOUSE_BUTTON_LEFT:
            window.context()->makeCurrent();
            m_nav.panBegin(event.pos());
            window.context()->doneCurrent();
            event.accept();
            break;

        case GLFW_MOUSE_BUTTON_RIGHT:
            window.context()->makeCurrent();
            m_nav.rotateBegin(event.pos());
            window.context()->doneCurrent();
            event.accept();
            break;

        default:
            break;
        }
    }
    virtual void mouseMoveEvent(
        Window & window
    ,   MouseEvent & event)
    {
        switch (m_nav.mode())
        {
        case WorldInHandNavigation::PanInteraction:
            window.context()->makeCurrent();
            m_nav.panProcess(event.pos());
            window.context()->doneCurrent();
            event.accept();
            break;
        
        case WorldInHandNavigation::RotateInteraction:
            window.context()->makeCurrent();
            m_nav.rotateProcess(event.pos());
            window.context()->doneCurrent();
            event.accept();

        default:break;
        }
    }
    virtual void mouseReleaseEvent(
        Window & window
    ,   MouseEvent & event)
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

    void scrollEvent(
        Window & window
    ,   ScrollEvent & event)
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

    return Window::run();
}
