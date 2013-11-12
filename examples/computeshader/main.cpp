
#include <GL/glew.h>

#include <algorithm>
#include <random>

#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/transform.hpp>

#include <glow/Uniform.h>
#include <glow/Array.h>
#include <glow/ShaderFile.h>
#include <glow/Program.h>
#include <glow/Shader.h>
#include <glow/Buffer.h>
#include <glow/VertexArrayObject.h>
#include <glow/VertexAttributeBinding.h>
#include <glowwindow/ContextFormat.h>
#include <glow/Error.h>
#include <glow/logging.h>
#include <glowutils/ScreenAlignedQuad.h>
#include <glowwindow/Context.h>
#include <glowwindow/Window.h>
#include <glowwindow/WindowEventHandler.h>

using namespace glow;

class EventHandler : public WindowEventHandler
{
public:
    EventHandler()
    :   m_frame(0)
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
        DebugMessageOutput::enable();

        glClearColor(0.2f, 0.3f, 0.4f, 1.f);

	    createAndSetupTexture();
	    createAndSetupShaders();
	    createAndSetupGeometry();
    }
    
    virtual void resizeEvent(ResizeEvent & event) override
    {
        int width = event.width();
        int height = event.height();
        int side = std::min<int>(width, height);
        glViewport((width - side) / 2, (height - side) / 2, side, side);
    }

    virtual void paintEvent(PaintEvent &) override
    {
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        ++m_frame %= 628; // int(pi * 100)

	    m_computeProgram->setUniform("roll", static_cast<float>(m_frame) * 0.01f);

	    m_texture->bind();

	    m_computeProgram->use();
	    glDispatchCompute(512/16, 512/16, 1); // 512^2 threads in blocks of 16^2
	    m_computeProgram->release();

        m_quad->draw();
    }

    virtual void idle(Window & window) override
    {
        window.repaint();
    }

    virtual void keyReleaseEvent(KeyEvent & event) override
    {
        if (GLFW_KEY_F5 == event.key())
            glow::ShaderFile::reloadAll();
    }

protected:
	glow::ref_ptr<glow::Texture> m_texture;

    glow::ref_ptr<glow::Program> m_computeProgram;
    glow::ref_ptr<glow::ScreenAlignedQuad> m_quad;

    unsigned int m_frame;
};


/** This example shows ... .
*/
int main(int argc, char* argv[])
{
    glewExperimental = GL_TRUE;

    ContextFormat format;

    Window window;
    window.assign(new EventHandler());

    window.create(format, "Compute Shader Example");
    window.show();
    window.context()->setSwapInterval(Context::NoVerticalSyncronization);

    return MainLoop::run();
}

void EventHandler::createAndSetupTexture()
{
	m_texture = new glow::Texture(GL_TEXTURE_2D);

    m_texture->setParameter(GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	m_texture->setParameter(GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	m_texture->image2D(0, GL_R32F, 512, 512, 0, GL_RED, GL_FLOAT, nullptr);
	m_texture->bindImageTexture(0, 0, GL_FALSE, 0, GL_WRITE_ONLY, GL_R32F);
}

void EventHandler::createAndSetupShaders()
{
    m_computeProgram = new glow::Program();
    m_computeProgram->attach(glow::Shader::fromFile(GL_COMPUTE_SHADER, "data/computeshader/cstest.comp"));

    m_computeProgram->setUniform("destTex", 0);
}

void EventHandler::createAndSetupGeometry()
{
    m_quad = new glow::ScreenAlignedQuad(m_texture);
    m_quad->setSamplerUniform(0);
}
