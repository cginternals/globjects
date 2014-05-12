


#include <algorithm>
#include <random>
#include <cassert>
#include <chrono>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/transform.hpp>
#include <glm/gtx/constants.hpp>

#include <glow/Error.h>
#include <glow/Uniform.h>
#include <glow/Program.h>
#include <glow/Shader.h>
#include <glow/Buffer.h>
#include <glow/logging.h>
#include <glow/VertexArrayObject.h>
#include <glow/debugmessageoutput.h>

#include <glowutils/Timer.h>
#include <glowutils/File.h>
#include <glowutils/AxisAlignedBoundingBox.h>
#include <glowutils/Icosahedron.h>
#include <glowutils/Camera.h>
#include <glowutils/AdaptiveGrid.h>
#include <glowutils/AbstractCoordinateProvider.h>
#include <glowutils/WorldInHandNavigation.h>
#include <glowutils/CameraPathRecorder.h>
#include <glowutils/CameraPathPlayer.h>
#include <glowutils/glowutils.h>
#include <glowutils/StringTemplate.h>

#include <glowwindow/ContextFormat.h>
#include <glowwindow/Context.h>
#include <glowwindow/Window.h>
#include <glowwindow/WindowEventHandler.h>

#include <ExampleWindowEventHandler.h>

using namespace glow;
using namespace glowwindow;
using namespace glowutils;
using namespace glm;


class EventHandler : public ExampleWindowEventHandler, AbstractCoordinateProvider
{
public:
    EventHandler()
    : m_camera(vec3(0.f, 1.f, 4.0f))
    , angle(0)
    , player(m_camera)
    , iterations(2)
    , usePath(false)
    {
        m_aabb.extend(vec3(-8.f, -1.f, -8.f));
        m_aabb.extend(vec3(8.f, 1.f, 8.f));

        m_nav.setCamera(&m_camera);
        m_nav.setCoordinateProvider(this);
        m_nav.setBoundaryHint(m_aabb);


        vec3 up(0.0, 1.0, 0.0);
        vec3 center(0.0, 0.0, 0.0);
        float fov = radians(60.0f);
        float height = 1.0;
        float d = 4.0;

        /*path
            << CameraPathPoint(vec3(0.0, 1.0, 4.0), center, up, fov)
            << CameraPathPoint(vec3(0.0, 1.0, 5.0), center, up, fov)
            << CameraPathPoint(vec3(2.0, 2.0, 5.0), center, vec3(-1.0, 0.0, 0.0), fov)
            << CameraPathPoint(vec3(1.0, 2.0, 4.0), center, up, fov)
            << CameraPathPoint(vec3(1.0, 2.0, 10.0), center, up, fov)
            << CameraPathPoint(vec3(0.0, 2.0, 4.0), center, up, fov)
            << CameraPathPoint(vec3(2.0, 2.0, 5.0), vec3(-5.0, -1.0, -1.0), vec3(1.0, 0.0, 0.0), fov)
            << CameraPathPoint(vec3(0.0, 1.0, 4.0), center, up, fov);*/

        path
            << CameraPathPoint(vec3(-1.0*d, height, -1.0*d), center, up, fov)
            << CameraPathPoint(vec3(-1.0*d, height, 1.0*d), vec3(0.0, 5.0, 0.0), vec3(-1.0, 0.0, 0.0), fov)
            << CameraPathPoint(vec3(1.0*d, height*2, 1.0*d), center, vec3(0.0, -1.0, 0.0), fov)
            << CameraPathPoint(vec3(1.0*d, height*6, 3.0*d), center, vec3(0.0, 0.0, 1.0), fov)
            << CameraPathPoint(vec3(-1.0*d, height, -4.0*d), center, up, fov)
            << CameraPathPoint(vec3(-1.0*d, height, -1.0*d), center, up, fov)
        ;

        /*path
            << CameraPathPoint(vec3(-1.0*d, height, -1.0*d), center, up, fov)
            << CameraPathPoint(vec3(-1.0*d, height, 1.0*d), center, up, fov)
            << CameraPathPoint(vec3(1.0*d, height, 1.0*d), center, up, fov)
            << CameraPathPoint(vec3(1.0*d, height, -1.0*d), center, up, fov)
            << CameraPathPoint(vec3(-1.0*d, height, -1.0*d), center, up, fov)
        ;*/

        player.setPath(path);
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

        debugmessageoutput::enable();

        gl::ClearColor(1.0f, 1.0f, 1.0f, 0.f);


        m_sphere = new Program();
      
        StringTemplate* vertexShaderSource = new StringTemplate(new File("data/adaptive-grid/sphere.vert"));
        StringTemplate* fragmentShaderSource = new StringTemplate(new File("data/adaptive-grid/sphere.frag"));

#ifdef MAC_OS
        vertexShaderSource->replace("#version 140", "#version 150");
        fragmentShaderSource->replace("#version 140", "#version 150");
#endif
      
        m_sphere->attach(
            new Shader(gl::VERTEX_SHADER, vertexShaderSource)
        ,   new Shader(gl::FRAGMENT_SHADER, fragmentShaderSource));

        m_icosahedron = new Icosahedron(iterations);
        m_agrid = new AdaptiveGrid(16U);

        m_camera.setZNear(0.1f);
        m_camera.setZFar(1024.f);

        m_agrid->setCamera(&m_camera);

        player.createVao();

        timer.start();

        window.addTimer(0, 0);
    }
    virtual void finalize(Window & ) override
    {
        m_sphere = nullptr;
        m_icosahedron = nullptr;
        m_agrid = nullptr;
        player.freeVao();
    }

    virtual void framebufferResizeEvent(ResizeEvent & event) override
    {
        gl::Viewport(0, 0, event.width(), event.height());
        m_camera.setViewport(event.width(), event.height());
    }

    virtual void paintEvent(PaintEvent &) override
    {
        gl::Clear(gl::COLOR_BUFFER_BIT | gl::DEPTH_BUFFER_BIT);

        m_agrid->update();
        m_sphere->setUniform("transform", m_camera.viewProjection());

        m_sphere->use();
        m_icosahedron->draw();
        m_sphere->release();

        player.draw(m_camera.viewProjection());

        m_agrid->draw();
    }

    virtual void keyPressEvent(KeyEvent & event) override
    {
        switch (event.key())
        {
            case GLFW_KEY_SLASH:
                if (iterations>1)
                    m_icosahedron = new Icosahedron(--iterations);
                break;
            case GLFW_KEY_RIGHT_BRACKET:
                if (iterations<7)
                    m_icosahedron = new Icosahedron(++iterations);
                break;
            case GLFW_KEY_T:
                usePath = !usePath;
                break;
            default:
                break;
        }
    }

    virtual void idle(Window & ) override
    {
    }

    virtual void timerEvent(TimerEvent & event) override
    {
        auto d = std::chrono::duration<long double, std::nano>(timer.elapsed());
        timer.reset();

        static auto duration = std::chrono::seconds(10);

        if (usePath)
        {
			angle += static_cast<float>(d.count() / std::chrono::duration_cast<std::chrono::duration<long double, std::nano>>(duration).count());

            while (angle>1)
                angle = 0;

            float t = angle;

            player.play(t);
        }

        event.window()->repaint();
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

            default:
                break;
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

    virtual void scrollEvent(ScrollEvent & event) override
    {
        if (WorldInHandNavigation::NoInteraction != m_nav.mode())
            return;

        m_nav.scaleAtMouse(event.pos(), -event.offset().y * 0.1f);
        event.accept();
    }

    virtual float depthAt(const ivec2 & windowCoordinates) const override
    {
        return AbstractCoordinateProvider::depthAt(m_camera, gl::DEPTH_COMPONENT, windowCoordinates);
    }

    virtual vec3 objAt(const ivec2 & windowCoordinates) const override
    {
        return unproject(m_camera, static_cast<gl::GLenum>(gl::DEPTH_COMPONENT), windowCoordinates);
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

    glow::ref_ptr<Program> m_sphere;

    glow::ref_ptr<Icosahedron> m_icosahedron;
    glow::ref_ptr<AdaptiveGrid> m_agrid;

    glowutils::Timer timer;
    Camera m_camera;
    float angle;
    CameraPath path;
    CameraPathPlayer player;
    unsigned iterations;

    bool usePath;

    WorldInHandNavigation m_nav;
    AxisAlignedBoundingBox m_aabb;
};


/** This example shows ... .
*/
int main(int /*argc*/, char* /*argv*/[])
{
    glow::info() << "Usage:";
    glow::info() << "\t" << "ESC" << "\t\t" << "Close example";
    glow::info() << "\t" << "ALT + Enter" << "\t" << "Toggle fullscreen";
    glow::info() << "\t" << "F11" << "\t\t" << "Toggle fullscreen";
    //glowbase::info() << "\t" << "F5" << "\t\t" << "Reload shaders";
    //glowbase::info() << "\t" << "Space" << "\t\t" << "Reset camera";
    glow::info() << "\t" << "Left Mouse" << "\t" << "Pan scene";
    glow::info() << "\t" << "Right Mouse" << "\t" << "Rotate scene";
    glow::info() << "\t" << "Mouse Wheel" << "\t" << "Zoom scene";
    glow::info() << "\t" << "T" << "\t\t" << "Toggle camera path usage";
    glow::info() << "\t" << "/" << "\t\t" << "Increase Icosahedron resolution";
    glow::info() << "\t" << "]" << "\t\t" << "Decrease Icosahedron resolution";

    ContextFormat format;
    format.setVersion(3, 0);
    format.setDepthBufferSize(16);

    Window window;
    window.setEventHandler(new EventHandler());

    if (window.create(format, "Camera Path Example"))
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
