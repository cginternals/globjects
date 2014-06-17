

#include <algorithm>
#include <random>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/transform.hpp>

#include <glow/logging.h>
#include <glow/Texture.h>
#include <glow/Program.h>
#include <glow/debugmessageoutput.h>

#include <glowutils/Camera.h>
#include <glowutils/ScreenAlignedQuad.h>
#include <glowutils/StringTemplate.h>
#include <glowbase/File.h>

#include <glowwindow/ContextFormat.h>
#include <glowwindow/Context.h>
#include <glowwindow/Window.h>
#include <glowwindow/WindowEventHandler.h>
#include <glowwindow/events.h>

#include "RawFile.h"

#include <ExampleWindowEventHandler.h>


using namespace glowwindow;
using namespace glm;

class EventHandler : public ExampleWindowEventHandler
{
public:
    EventHandler()
    : m_camera(vec3(0.f, 1.f, 4.0f))
    , m_angle(0.f)
    , m_scale(16.f)
    {
    }

    virtual ~EventHandler()
    {
    }

    void createAndSetupTexture();
	void createAndSetupGeometry();

    virtual void initialize(Window & window) override
    {
        ExampleWindowEventHandler::initialize(window);

        glow::debugmessageoutput::enable();

        gl::glClearColor(0.2f, 0.3f, 0.4f, 1.f);


        m_camera.setZNear(0.1f);
        m_camera.setZFar(32.f);


        createAndSetupTexture();
	    createAndSetupGeometry();
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


        m_program->setUniform("modelViewProjection", m_camera.viewProjection());

        mat4 R = glm::rotate(m_angle, glm::vec3(1.f,0.f,0.f));

        m_program->setUniform("R", R);
        m_program->setUniform("S", m_scale);

        m_quad->draw();
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
            glow::File::reloadAll();
            break;
        case GLFW_KEY_UP:
            m_angle += 0.5f;
            break;
        case GLFW_KEY_DOWN:
            m_angle -= 0.5f;
            break;
        case GLFW_KEY_MINUS:
            m_scale *= 1.1f;
            break; 
        case GLFW_KEY_EQUAL: // plus key? (de layout)
            m_scale /= 1.1f;
            break;
        }
    }

protected:
    glow::ref_ptr<glow::Texture> m_texture;
    glow::ref_ptr<glow::Program> m_program;

    glowutils::Camera m_camera;
    float m_angle, m_scale;

    glow::ref_ptr<glowutils::ScreenAlignedQuad> m_quad;
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
    glow::info() << "\t" << "UP" << "\t\t" << "Tilt up";
    glow::info() << "\t" << "DOWN" << "\t\t" << "Tilt down";
    glow::info() << "\t" << "-" << "\t\t" << "Move forward";
    glow::info() << "\t" << "=" << "\t\t" << "Move backward";

    ContextFormat format;
    format.setVersion(3, 2);

    Window window;

    window.setEventHandler(new EventHandler());

    if (window.create(format, "MipMap Filtering Example"))
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

void EventHandler::createAndSetupTexture()
{
	m_texture = new glow::Texture(gl::GL_TEXTURE_2D);

    m_texture->setParameter(gl::GL_TEXTURE_MIN_FILTER, static_cast<gl::GLint>(gl::GL_LINEAR_MIPMAP_LINEAR));
    m_texture->setParameter(gl::GL_TEXTURE_MAG_FILTER, static_cast<gl::GLint>(gl::GL_LINEAR));

    m_texture->setParameter(gl::GL_TEXTURE_WRAP_S, static_cast<gl::GLint>(gl::GL_REPEAT));
    m_texture->setParameter(gl::GL_TEXTURE_WRAP_T, static_cast<gl::GLint>(gl::GL_REPEAT));

    RawFile raw("data/mipmap-filtering/grass.256.256.dxt1-rgb.raw");
    if (!raw.isValid())
        return;

    m_texture->compressedImage2D(0, gl::GL_COMPRESSED_RGB_S3TC_DXT1_EXT, glm::ivec2(256, 256), 0, static_cast<gl::GLsizei>(raw.size()), raw.data());
    gl::glGenerateMipmap(gl::GL_TEXTURE_2D);
}

void EventHandler::createAndSetupGeometry()
{
    glowutils::StringTemplate * sphereVertexShader = new glowutils::StringTemplate(new glow::File("data/mipmap-filtering/mipmap.vert"));
    glowutils::StringTemplate * sphereFragmentShader = new glowutils::StringTemplate(new glow::File("data/mipmap-filtering/mipmap.frag"));

    m_program = new glow::Program();
    m_program->attach(new glow::Shader(gl::GL_VERTEX_SHADER, sphereVertexShader), new glow::Shader(gl::GL_FRAGMENT_SHADER, sphereFragmentShader));

	m_quad = new glowutils::ScreenAlignedQuad(m_program);
    m_quad->setSamplerUniform(0);

    m_program->setUniform("source", 0);
}
