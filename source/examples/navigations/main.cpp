


#include <algorithm>
#include <random>
#include <cassert>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/transform.hpp>

#include <glbinding/gl/gl.h>

#include <globjects/Uniform.h>
#include <globjects/Program.h>
#include <globjects/Shader.h>
#include <globjects/Buffer.h>
#include <globjects/logging.h>
#include <globjects/VertexArray.h>
#include <globjects/DebugMessage.h>

#include <globjects-utils/Timer.h>
#include <globjects-utils/AxisAlignedBoundingBox.h>
#include <globjects-utils/Icosahedron.h>
#include <globjects-utils/Camera.h>
#include <globjects-utils/AdaptiveGrid.h>
#include <globjects-utils/AbstractCoordinateProvider.h>
#include <globjects-utils/WorldInHandNavigation.h>
#include <globjects-utils/FlightNavigation.h>
#include <globjects-base/File.h>
#include <globjects-base/File.h>
#include <globjects-utils/globjects-utils.h>
#include <globjects-utils/StringTemplate.h>

#include <common/ContextFormat.h>
#include <common/Context.h>
#include <common/Window.h>
#include <common/WindowEventHandler.h>
#include <common/events.h>

#include <ExampleWindowEventHandler.h>


using namespace glm;


class EventHandler : public ExampleWindowEventHandler, gloutils::AbstractCoordinateProvider
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
        ExampleWindowEventHandler::initialize(window);

        glo::DebugMessage::enable();

        gl::glClearColor(1.0f, 1.0f, 1.0f, 0.f);


        m_sphere = new glo::Program();
        gloutils::StringTemplate* vertexShaderSource = new gloutils::StringTemplate(new glo::File("data/adaptive-grid/sphere.vert"));
        gloutils::StringTemplate* fragmentShaderSource = new gloutils::StringTemplate(new glo::File("data/adaptive-grid/sphere.frag"));

#ifdef MAC_OS
        vertexShaderSource->replace("#version 140", "#version 150");
        fragmentShaderSource->replace("#version 140", "#version 150");
#endif
        
        m_sphere->attach(
            new glo::Shader(gl::GL_VERTEX_SHADER, vertexShaderSource)
        ,   new glo::Shader(gl::GL_FRAGMENT_SHADER, fragmentShaderSource));
        

        m_icosahedron = new gloutils::Icosahedron(2);
        m_agrid = new gloutils::AdaptiveGrid(16U);

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
        gl::glViewport(0, 0, event.width(), event.height());


        m_camera.setViewport(event.width(), event.height());
    }

    virtual void paintEvent(PaintEvent &) override
    {
        gl::glClear(gl::GL_COLOR_BUFFER_BIT | gl::GL_DEPTH_BUFFER_BIT);


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
            glo::File::reloadAll();
            break;
        case GLFW_KEY_1:
            m_flightEnabled = !m_flightEnabled;
            if (!m_flightEnabled)
                m_flightNav.stopMovement(gloutils::FlightNavigation::All);
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
                m_flightNav.startMovement(gloutils::FlightNavigation::Forward);
                break;
            case GLFW_KEY_A:
            case GLFW_KEY_LEFT:
                m_flightNav.startMovement(gloutils::FlightNavigation::Left);
                break;
            case GLFW_KEY_S:
            case GLFW_KEY_DOWN:
                m_flightNav.startMovement(gloutils::FlightNavigation::Backward);
                break;
            case GLFW_KEY_D:
            case GLFW_KEY_RIGHT:
                m_flightNav.startMovement(gloutils::FlightNavigation::Right);
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
                m_flightNav.stopMovement(gloutils::FlightNavigation::Forward);
                break;
            case GLFW_KEY_A:
            case GLFW_KEY_LEFT:
                m_flightNav.stopMovement(gloutils::FlightNavigation::Left);
                break;
            case GLFW_KEY_S:
            case GLFW_KEY_DOWN:
                m_flightNav.stopMovement(gloutils::FlightNavigation::Backward);
                break;
            case GLFW_KEY_D:
            case GLFW_KEY_RIGHT:
                m_flightNav.stopMovement(gloutils::FlightNavigation::Right);
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
        case gloutils::WorldInHandNavigation::PanInteraction:
            m_nav.panProcess(event.pos());
            event.accept();
            break;

        case gloutils::WorldInHandNavigation::RotateInteraction:
            m_nav.rotateProcess(event.pos());
            event.accept();
            break;
        case gloutils::WorldInHandNavigation::NoInteraction:
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

        if (gloutils::WorldInHandNavigation::NoInteraction != m_nav.mode())
            return;

        m_nav.scaleAtMouse(event.pos(), -event.offset().y * 0.1f);
        event.accept();
    }

    virtual float depthAt(const ivec2 & windowCoordinates) const override
    {
        return AbstractCoordinateProvider::depthAt(m_camera, gl::GL_DEPTH_COMPONENT, windowCoordinates);
    }

    virtual vec3 objAt(const ivec2 & windowCoordinates) const override
    {
        return unproject(m_camera, static_cast<gl::GLenum>(gl::GL_DEPTH_COMPONENT), windowCoordinates);
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

    glo::ref_ptr<glo::Program> m_sphere;
    
    glo::ref_ptr<gloutils::Icosahedron> m_icosahedron;
    glo::ref_ptr<gloutils::AdaptiveGrid> m_agrid;

    gloutils::Camera m_camera;
    gloutils::WorldInHandNavigation m_nav;
    gloutils::FlightNavigation m_flightNav;
    glm::ivec2 m_lastMousePos;
    bool m_flightEnabled;
    gloutils::Timer m_timer;

    gloutils::AxisAlignedBoundingBox m_aabb;
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
    glo::info() << "\t" << "Space" << "\t\t" << "Reset camera";
    glo::info() << "\t" << "1" << "\t" << "Toggle flight mode / world in hand navigation";

    glo::info();
    glo::info() << "\t" << "During world in hand navigation";
    glo::info() << "\t" << "Left Mouse" << "\t" << "Pan scene";
    glo::info() << "\t" << "Right Mouse" << "\t" << "Rotate scene";
    glo::info() << "\t" << "Mouse Wheel" << "\t" << "Zoom scene";

    glo::info();
    glo::info() << "\t" << "During flight mode";
    glo::info() << "\t" << "Mouse Movement" << "\t" << "Look around";
    glo::info() << "\t" << "W" << "\t\t" << "Move forward";
    glo::info() << "\t" << "UP" << "\t\t" << "Move forward";
    glo::info() << "\t" << "A" << "\t\t" << "Move left";
    glo::info() << "\t" << "LEFT" << "\t\t" << "Move left";
    glo::info() << "\t" << "S" << "\t\t" << "Move backward";
    glo::info() << "\t" << "DOWN" << "\t\t" << "Move backward";
    glo::info() << "\t" << "D" << "\t\t" << "Move right";
    glo::info() << "\t" << "RIGHT" << "\t\t" << "Move right";

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
