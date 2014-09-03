

#include <algorithm>
#include <random>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/transform.hpp>

#include <glbinding/gl/gl.h>

#include <globjects/logging.h>
#include <globjects/Texture.h>
#include <globjects/Program.h>
#include <globjects/DebugMessage.h>

#include <globjects-utils/Camera.h>
#include <globjects-utils/ScreenAlignedQuad.h>
#include <globjects-utils/StringTemplate.h>
#include <globjects-base/File.h>

#include <common/ContextFormat.h>
#include <common/Context.h>
#include <common/Window.h>
#include <common/WindowEventHandler.h>
#include <common/events.h>

#include "RawFile.h"

#include <ExampleWindowEventHandler.h>



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

        glo::DebugMessage::enable();

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
            glo::File::reloadAll();
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
    glo::ref_ptr<glo::Texture> m_texture;
    glo::ref_ptr<glo::Program> m_program;

    gloutils::Camera m_camera;
    float m_angle, m_scale;

    glo::ref_ptr<gloutils::ScreenAlignedQuad> m_quad;
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
    glo::info() << "\t" << "UP" << "\t\t" << "Tilt up";
    glo::info() << "\t" << "DOWN" << "\t\t" << "Tilt down";
    glo::info() << "\t" << "-" << "\t\t" << "Move forward";
    glo::info() << "\t" << "=" << "\t\t" << "Move backward";

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
	m_texture = new glo::Texture(gl::GL_TEXTURE_2D);

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
    gloutils::StringTemplate * sphereVertexShader = new gloutils::StringTemplate(new glo::File("data/mipmap-filtering/mipmap.vert"));
    gloutils::StringTemplate * sphereFragmentShader = new gloutils::StringTemplate(new glo::File("data/mipmap-filtering/mipmap.frag"));

    m_program = new glo::Program();
    m_program->attach(new glo::Shader(gl::GL_VERTEX_SHADER, sphereVertexShader), new glo::Shader(gl::GL_FRAGMENT_SHADER, sphereFragmentShader));

	m_quad = new gloutils::ScreenAlignedQuad(m_program);
    m_quad->setSamplerUniform(0);

    m_program->setUniform("source", 0);
}
