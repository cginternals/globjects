
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

#include <glowutils/MathMacros.h>
#include <glowutils/Icosahedron.h>
#include <glowutils/UnitCube.h>
#include <glowutils/AdaptiveGrid.h>
#include <glowutils/AxonometricLookAt.h>

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
    : m_iso(false)
    {
        m_eye    = vec3( 0.f, 1.f, 4.0f);
        m_center = vec3( 0.f, 0.f, 0.f);
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

        m_sphere->bindFragDataLocation(0, "fragColor");

        m_icosahedron = new Icosahedron(2);
        m_agrid = new AdaptiveGrid(16U);
    }    

    virtual void resizeEvent(
        Window & window
    ,   unsigned int width
    ,   unsigned int height)
    {
        glViewport(0, 0, width, height);

        if (!m_sphere)
            return;

        const float aspect(static_cast<float>(width) / static_cast<float>(height));
        m_projection = perspective(40.f, aspect, 0.1f, 1024.f);
        m_ortho = ortho(-aspect, aspect, -1.f, 1.f,  0.1f, 1024.f);

        m_agrid->setNearFar(0.1f, 1024.f);

        m_axonometric.setPosition(m_eye);
        const mat4 view(m_iso ? m_axonometric.matrix() : lookAt(m_eye, m_center, vec3(0.0, 1.0, 0.0)));


        m_sphere->setUniform("transform", (m_iso ? m_ortho : m_projection) * view);
        m_agrid->update(m_eye, (m_iso ? m_ortho : m_projection) * view);
    }

    virtual void paintEvent(Window & window)
    {
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        const mat4 view(m_iso ? m_axonometric.matrix() : lookAt(m_eye, m_center, vec3(0.0, 1.0, 0.0)));
        
        m_agrid->update(m_eye, (m_iso ? m_ortho : m_projection) * view);
        m_sphere->setUniform("transform", (m_iso ? m_ortho : m_projection) * view);


        m_sphere->use();
        m_icosahedron->draw();
        //m_unitcube->draw();
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
            case GLFW_KEY_SPACE:
                m_iso = !m_iso;
                break;
            case GLFW_KEY_1:
                m_center.x += d;
                m_eye.x += d;
                break;
            case GLFW_KEY_2:
                m_eye.x -= d;
                m_center.x -= d;
                break;
            case GLFW_KEY_3:
                m_eye += (m_eye - m_center) * 0.1f;
                break;
            case GLFW_KEY_4:
                m_eye -= (m_eye - m_center) * 0.1f;
                break;
            case GLFW_KEY_5:
                m_center.z += d;
                m_eye.z += d;
                break;
            case GLFW_KEY_6:
                m_center.z -= d;
                m_eye.z -= d;
                break;
        }

        m_axonometric.setPosition(m_eye);
    }

protected:
    ref_ptr<Program> m_sphere;

    ref_ptr<Icosahedron> m_icosahedron;

    ref_ptr<AdaptiveGrid> m_agrid;

    vec3 m_eye;
    vec3 m_center;

    mat4 m_projection;
    mat4 m_ortho;

    bool m_iso;

    AxonometricLookAt m_axonometric;
};


/** This example shows ... .
*/
int main(int argc, char** argv)
{
    glewExperimental = GL_TRUE;

    ContextFormat format;
    format.setVersion(4, 0);
    format.setProfile(ContextFormat::CoreProfile);

    Window window;
    window.assign(new EventHandler());

    window.create(format, "Adaptive Grid Example");
    window.context()->setSwapInterval(Context::VerticalSyncronization);
    window.show();

    return Window::run();
}
