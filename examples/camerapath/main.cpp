
#include <GL/glew.h>

#include <algorithm>
#include <random>
#include <cassert>
#include <chrono>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/transform.hpp>
#include <glm/gtc/constants.hpp>

#include <glow/Error.h>
#include <glow/Uniform.h>
#include <glow/ShaderFile.h>
#include <glow/Program.h>
#include <glow/Shader.h>
#include <glow/Buffer.h>
#include <glow/logging.h>
#include <glow/AutoTimer.h>
#include <glow/Timer.h>
#include <glow/VertexArrayObject.h>

#include <glowutils/File.h>
#include <glowutils/AxisAlignedBoundingBox.h>
#include <glowutils/MathMacros.h>
#include <glowutils/Icosahedron.h>
#include <glowutils/Camera.h>
#include <glowutils/AdaptiveGrid.h>
#include <glowutils/AbstractCoordinateProvider.h>
#include <glowutils/WorldInHandNavigation.h>
#include <glowutils/CameraPathRecorder.h>

#include <glowwindow/ContextFormat.h>
#include <glowwindow/Context.h>
#include <glowwindow/Window.h>
#include <glowwindow/WindowEventHandler.h>

using namespace glow;
using namespace glm;


class EventHandler : public WindowEventHandler
{
public:
    EventHandler()
    : m_camera(vec3(0.f, 1.f, 4.0f))
    , angle(0)
    , player(m_camera)
    , iterations(2)
    {
        vec3 up(0.0, 1.0, 0.0);
        vec3 center(0.0, 0.0, 0.0);
        float fov = 60.0;
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

        /*path
            << CameraPathPoint(vec3(-1.0*d, height, -1.0*d), center, up, fov)
            << CameraPathPoint(vec3(-1.0*d, height, 1.0*d), center, vec3(-1.0, 0.0, 0.0), fov)
            << CameraPathPoint(vec3(1.0*d, height, 1.0*d), center, vec3(0.0, -1.0, 0.0), fov)
            << CameraPathPoint(vec3(1.0*d, height, -1.0*d), center, vec3(0.0, 0.0, -1.0), fov)
            << CameraPathPoint(vec3(-1.0*d, height, -1.0*d), center, up, fov);*/

        path
            << CameraPathPoint(vec3(-1.0*d, height, -1.0*d), center, up, fov)
            << CameraPathPoint(vec3(-1.0*d, height, 1.0*d), center, up, fov)
            << CameraPathPoint(vec3(1.0*d, height, 1.0*d), center, up, fov)
            << CameraPathPoint(vec3(1.0*d, height, -1.0*d), center, up, fov)
            << CameraPathPoint(vec3(-1.0*d, height, -1.0*d), center, up, fov);

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
        DebugMessageOutput::enable();

        glClearColor(1.0f, 1.0f, 1.0f, 0.f);


        m_sphere = new Program();
        m_sphere->attach(
            createShaderFromFile(GL_VERTEX_SHADER,   "data/adaptive-grid/sphere.vert")
        ,   createShaderFromFile(GL_FRAGMENT_SHADER, "data/adaptive-grid/sphere.frag"));

        m_icosahedron = new Icosahedron(iterations);
        m_agrid = new AdaptiveGrid(16U);

        m_camera.setZNear(0.1f);
        m_camera.setZFar(1024.f);

        m_agrid->setCamera(&m_camera);

        timer.start();

        window.addTimer(0, 0);
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
        }
        glow::debug() << event.key();
    }

    virtual void idle(Window & window) override
    {
        //window.repaint();
    }

    virtual void timerEvent(TimerEvent & event) override
    {
        /*angle += radians(1.f);
        while (angle >= 2*glm::pi<float>())
            angle -= 2*pi<float>();

        vec2 pos(sin(angle), cos(angle));
        pos *= 4.0;

        m_camera.setEye(vec3(pos.x, 1.0f + sin(10*angle)/4.0, pos.y));*/

        auto d = std::chrono::duration<long double, std::nano>(timer.elapsed());
        timer.reset();

        static auto duration = std::chrono::seconds(5);

        angle+= d/std::chrono::duration_cast<std::chrono::duration<long double, std::nano>>(duration);

        while (angle>1)
            angle = 0;

        float t = angle;

        player.play(t);

        event.window()->repaint();
    }

protected:

    ref_ptr<Program> m_sphere;

    ref_ptr<Icosahedron> m_icosahedron;
    ref_ptr<AdaptiveGrid> m_agrid;

    Timer timer;
    Camera m_camera;
    float angle;
    CameraPath path;
    CameraPathPlayer player;
    unsigned iterations;
};


/** This example shows ... .
*/
int main(int argc, char* argv[])
{
    ContextFormat format;
    format.setVersion(4, 0);
    format.setProfile(ContextFormat::CoreProfile);
    format.setDepthBufferSize(16);

    Window window;
    window.setEventHandler(new EventHandler());

    window.create(format, "Camera Path Example");
    window.context()->setSwapInterval(Context::VerticalSyncronization);
    window.show();

    return MainLoop::run();
}
