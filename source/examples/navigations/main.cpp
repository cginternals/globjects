
#include <GL/glew.h>

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
#include <glow/VertexArrayObject.h>
#include <glow/debugmessageoutput.h>

#include <glowutils/Timer.h>
#include <glowutils/AxisAlignedBoundingBox.h>
#include <glowutils/Icosahedron.h>
#include <glowutils/Camera.h>
#include <glowutils/AdaptiveGrid.h>
#include <glowutils/AbstractCoordinateProvider.h>
#include <glowutils/WorldInHandNavigation.h>
#include <glowutils/FlightNavigation.h>
#include <glowutils/File.h>
#include <glowutils/File.h>
#include <glowutils/glowutils.h>
#include <glowutils/StringTemplate.h>

#include <glowwindow/ContextFormat.h>
#include <glowwindow/Context.h>
#include <glowwindow/Window.h>
#include <glowwindow/WindowEventHandler.h>
#include <glowwindow/events.h>

#include <ExampleWindowEventHandler.h>

using namespace glowwindow;
using namespace glm;


class EventHandler : public ExampleWindowEventHandler, glowutils::AbstractCoordinateProvider
{
public:
    EventHandler()
    : m_camera(vec3(0.f, 1.f, 4.0f))
    , m_flightEnabled(false)
    {
        m_aabb.extend(vec3(-8.f, -1.f, -8.f));
        m_aabb.extend(vec3(8.f, 1.f, 8.f));

        m_nav.setCamera(&m_camera);
        m_nav.setCoordinateProvider(this);
        m_nav.setBoundaryHint(m_aabb);

        m_flightNav.setCamera(&m_camera);

        m_timer.start();
    }

    virtual ~EventHandler()
    {
    }

    void createAndSetupTexture();
    void createAndSetupShaders();
	void createAndSetupGeometry();

    virtual void initialize(Window & window) override
    {
        glow::debugmessageoutput::enable();

        glClearColor(1.0f, 1.0f, 1.0f, 0.f);
        CheckGLError();

        m_sphere = new glow::Program();
        glowutils::StringTemplate* vertexShaderSource = new glowutils::StringTemplate(new glowutils::File("data/adaptive-grid/sphere.vert"));
        glowutils::StringTemplate* fragmentShaderSource = new glowutils::StringTemplate(new glowutils::File("data/adaptive-grid/sphere.frag"));

#ifdef MAC_OS
        vertexShaderSource->replace("#version 140", "#version 150");
        fragmentShaderSource->replace("#version 140", "#version 150");
#endif
        
        m_sphere->attach(
            new glow::Shader(GL_VERTEX_SHADER, vertexShaderSource)
        ,   new glow::Shader(GL_FRAGMENT_SHADER, fragmentShaderSource));
        

        m_icosahedron = new glowutils::Icosahedron(2);
        m_agrid = new glowutils::AdaptiveGrid(16U);

        m_camera.setZNear(0.1f);
        m_camera.setZFar(1024.f);

        m_agrid->setCamera(&m_camera);

        window.addTimer(0, 0, false);
    }
    virtual void finalize(Window &) override
    {
        m_sphere = nullptr;
        m_icosahedron = nullptr;
        m_agrid = nullptr;
    }

    virtual void framebufferResizeEvent(ResizeEvent & event) override
    {
        glViewport(0, 0, event.width(), event.height());
        CheckGLError();

        m_camera.setViewport(event.width(), event.height());
    }

    virtual void paintEvent(PaintEvent &) override
    {
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        CheckGLError();

        m_agrid->update();
        m_sphere->setUniform("transform", m_camera.viewProjection());

        m_sphere->use();
        m_icosahedron->draw();
        m_sphere->release();

        m_agrid->draw();
    }

    virtual void timerEvent(TimerEvent & event) override
    {
        float delta = static_cast<float>(static_cast<float>(m_timer.elapsed().count())/1000.0/1000.0/1000.0);
        m_timer.reset();
        m_flightNav.move(delta);

        event.window()->repaint();
    }

    virtual void keyPressEvent(KeyEvent & event) override
    {
        switch (event.key())
        {
        case GLFW_KEY_F5:
            glowutils::File::reloadAll();
            break;
        case GLFW_KEY_1:
            m_flightEnabled = !m_flightEnabled;
            if (!m_flightEnabled)
                m_flightNav.stopMovement(glowutils::FlightNavigation::All);
            event.window()->setInputMode(GLFW_CURSOR, m_flightEnabled ? GLFW_CURSOR_DISABLED : GLFW_CURSOR_NORMAL);
            break;
        case GLFW_KEY_SPACE:
            m_camera.setCenter(vec3());
            m_camera.setEye(vec3(0.f, 1.f, 4.0f));
            m_camera.setUp(vec3(0,1,0));
            break;
        }

        if (!m_flightEnabled)
            return;
        switch (event.key())
        {
            case GLFW_KEY_W:
            case GLFW_KEY_UP:
                m_flightNav.startMovement(glowutils::FlightNavigation::Forward);
                break;
            case GLFW_KEY_A:
            case GLFW_KEY_LEFT:
                m_flightNav.startMovement(glowutils::FlightNavigation::Left);
                break;
            case GLFW_KEY_S:
            case GLFW_KEY_DOWN:
                m_flightNav.startMovement(glowutils::FlightNavigation::Backward);
                break;
            case GLFW_KEY_D:
            case GLFW_KEY_RIGHT:
                m_flightNav.startMovement(glowutils::FlightNavigation::Right);
                break;
        }
    }
    virtual void keyReleaseEvent(KeyEvent & event) override
    {
        if (!m_flightEnabled)
            return;
        switch (event.key())
        {
            case GLFW_KEY_W:
            case GLFW_KEY_UP:
                m_flightNav.stopMovement(glowutils::FlightNavigation::Forward);
                break;
            case GLFW_KEY_A:
            case GLFW_KEY_LEFT:
                m_flightNav.stopMovement(glowutils::FlightNavigation::Left);
                break;
            case GLFW_KEY_S:
            case GLFW_KEY_DOWN:
                m_flightNav.stopMovement(glowutils::FlightNavigation::Backward);
                break;
            case GLFW_KEY_D:
            case GLFW_KEY_RIGHT:
                m_flightNav.stopMovement(glowutils::FlightNavigation::Right);
                break;
        }
    }

    virtual void mousePressEvent(MouseEvent & event) override
    {
        if (m_flightEnabled)
            return;

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
        if (m_flightEnabled)
        {
            m_flightNav.mouseMove((event.pos()-m_lastMousePos));
            m_lastMousePos = event.pos();
            event.accept();
            return;
        }

        switch (m_nav.mode())
        {
        case glowutils::WorldInHandNavigation::PanInteraction:
            m_nav.panProcess(event.pos());
            event.accept();
            break;

        case glowutils::WorldInHandNavigation::RotateInteraction:
            m_nav.rotateProcess(event.pos());
            event.accept();
            break;
        case glowutils::WorldInHandNavigation::NoInteraction:
            break;
        }
    }
    virtual void mouseReleaseEvent(MouseEvent & event) override
    {
        if (m_flightEnabled)
            return;

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

    virtual void scrollEvent(ScrollEvent & event) override
    {
        if (m_flightEnabled)
            return;

        if (glowutils::WorldInHandNavigation::NoInteraction != m_nav.mode())
            return;

        m_nav.scaleAtMouse(event.pos(), -event.offset().y * 0.1f);
        event.accept();
    }

    virtual float depthAt(const ivec2 & windowCoordinates) const override
    {
        return AbstractCoordinateProvider::depthAt(m_camera, GL_DEPTH_COMPONENT, windowCoordinates);
    }

    virtual vec3 objAt(const ivec2 & windowCoordinates) const override
    {
        return unproject(m_camera, static_cast<GLenum>(GL_DEPTH_COMPONENT), windowCoordinates);
    }

    virtual vec3 objAt(const ivec2 & windowCoordinates, const float depth) const override
    {
        return unproject(m_camera, depth, windowCoordinates);
    }

    virtual glm::vec3 objAt(
        const ivec2 & windowCoordinates
    ,   const float depth
    ,   const mat4 & viewProjectionInverted) const override
    {
        return unproject(m_camera, viewProjectionInverted, depth, windowCoordinates);
    }

protected:

    glow::ref_ptr<glow::Program> m_sphere;
    
    glow::ref_ptr<glowutils::Icosahedron> m_icosahedron;
    glow::ref_ptr<glowutils::AdaptiveGrid> m_agrid;

    glowutils::Camera m_camera;
    glowutils::WorldInHandNavigation m_nav;
    glowutils::FlightNavigation m_flightNav;
    glm::ivec2 m_lastMousePos;
    bool m_flightEnabled;
    glowutils::Timer m_timer;

    glowutils::AxisAlignedBoundingBox m_aabb;
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
    glow::info() << "\t" << "Space" << "\t\t" << "Reset camera";
    glow::info() << "\t" << "1" << "\t" << "Toggle flight mode / world in hand navigation";

    glow::info();
    glow::info() << "\t" << "During world in hand navigation";
    glow::info() << "\t" << "Left Mouse" << "\t" << "Pan scene";
    glow::info() << "\t" << "Right Mouse" << "\t" << "Rotate scene";
    glow::info() << "\t" << "Mouse Wheel" << "\t" << "Zoom scene";

    glow::info();
    glow::info() << "\t" << "During flight mode";
    glow::info() << "\t" << "Mouse Movement" << "\t" << "Look around";
    glow::info() << "\t" << "W" << "\t\t" << "Move forward";
    glow::info() << "\t" << "UP" << "\t\t" << "Move forward";
    glow::info() << "\t" << "A" << "\t\t" << "Move left";
    glow::info() << "\t" << "LEFT" << "\t\t" << "Move left";
    glow::info() << "\t" << "S" << "\t\t" << "Move backward";
    glow::info() << "\t" << "DOWN" << "\t\t" << "Move backward";
    glow::info() << "\t" << "D" << "\t\t" << "Move right";
    glow::info() << "\t" << "RIGHT" << "\t\t" << "Move right";

    ContextFormat format;
    format.setVersion(3, 0);

    Window window;

    window.setEventHandler(new EventHandler());

    if (window.create(format, "Navigations Example"))
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
