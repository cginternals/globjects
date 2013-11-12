
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
#include <glow/Timer.h>

#include <glowutils/MathMacros.h>
#include <glowutils/Icosahedron.h>
#include <glowutils/AdaptiveGrid.h>
#include <glowutils/Camera.h>

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
    {
    }

    virtual ~EventHandler()
    {
    }

    void createAndSetupTexture();
    void createAndSetupShaders();
    void createAndSetupGeometry();

    virtual void initialize(Window & window) override
    {
        glEnable(GL_TEXTURE_2D);

        DebugMessageOutput::enable();

        glClearColor(1.0f, 1.0f, 1.0f, 0.f);

        m_sphere = new Program();
        m_sphere->attach(
            Shader::fromFile(GL_VERTEX_SHADER, "data/tessellation/sphere.vert")
        ,   Shader::fromFile(GL_TESS_CONTROL_SHADER, "data/tessellation/sphere.tcs")
        ,   Shader::fromFile(GL_TESS_EVALUATION_SHADER, "data/tessellation/sphere.tes")
        ,   Shader::fromFile(GL_GEOMETRY_SHADER, "data/tessellation/sphere.geom")
        ,   Shader::fromFile(GL_FRAGMENT_SHADER, "data/tessellation/sphere.frag")
        ,   Shader::fromFile(GL_FRAGMENT_SHADER, "data/common/phong.frag"));

        m_icosahedron = new Icosahedron();
        m_agrid = new AdaptiveGrid(16U);

        m_time.reset();
        m_time.start();

        m_camera.setZNear(0.1f);
        m_camera.setZFar(16.f);

        m_agrid->setCamera(&m_camera);
    }

    virtual void resizeEvent(ResizeEvent & event) override
    {
        int width = event.width();
        int height = event.height();

        glViewport(0, 0, width, height);
        m_camera.setViewport(width, height);
    }

    virtual void paintEvent(PaintEvent &) override
    {
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        m_agrid->update();

        float t = static_cast<float>(m_time.elapsed() * 4e-10f);
        mat4 R = glm::rotate(t * 10.f, vec3(sin(t * 0.321), cos(t * 0.234), sin(t * 0.123)));

        m_sphere->setUniform("transform", m_camera.viewProjection());
        m_sphere->setUniform("rotation", R);

        int level = int((sin(t) * 0.5 + 0.5) * 16) + 1;
        m_sphere->setUniform("level", level);

        m_sphere->use();
        glPatchParameteri(GL_PATCH_VERTICES, 3);
        m_icosahedron->draw(GL_PATCHES);
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
            glow::ShaderFile::reloadAll();
            break;
        }
    }

protected:
    ref_ptr<Program> m_sphere;

    ref_ptr<Icosahedron> m_icosahedron;
    ref_ptr<AdaptiveGrid> m_agrid;

    Camera m_camera;
    Timer m_time;

    vec3 m_rand;
};


/** This example shows ... .
*/
int main(int argc, char* argv[])
{
    glewExperimental = GL_TRUE;

    ContextFormat format;
    format.setVersion(4, 2);
    format.setProfile(ContextFormat::CoreProfile);
    format.setDepthBufferSize(16);

    Window window;
    window.assign(new EventHandler());

    window.create(format, "Post Processing Example");
    window.context()->setSwapInterval(Context::VerticalSyncronization);
    window.show();

    return MainLoop::run();
}
