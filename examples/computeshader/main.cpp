
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

    virtual void initializeEvent(Window & window)
    {
        DebugMessageOutput::enable();

        glClearColor(0.2f, 0.3f, 0.4f, 1.f);

	    createAndSetupTexture();
	    createAndSetupShaders();
	    createAndSetupGeometry();
    }
    
    virtual void resizeEvent(
        Window & window
    ,   const unsigned int width
    ,   const unsigned int height)
    {
    	int side = std::min<int>(width, height);
	    glViewport((width - side) / 2, (height - side) / 2, side, side);

	    m_program->setUniform("modelView", glm::mat4());
	    m_program->setUniform("projection", glm::ortho(0.f, 1.f, 0.f, 1.f, 0.f, 1.f));
    }

    virtual void paintEvent(Window & window)
    {
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        ++m_frame %= 628;

	    m_computeProgram->setUniform("roll", static_cast<float>(m_frame) * 0.01f);

	    m_texture->bind();

	    m_computeProgram->use();
	    glDispatchCompute(512/16, 512/16, 1); // 512^2 threads in blocks of 16^2
	    m_computeProgram->release();

	    m_program->use();

        m_vao->drawArrays(GL_TRIANGLE_FAN, 0, 4);

        m_program->release();
        m_texture->unbind();
    }

    virtual void idleEvent(Window & window)
    {
        window.repaint();
    }

    virtual void keyReleaseEvent(
        Window & window
    ,   KeyEvent & event)
    {
        if (GLFW_KEY_F5 == event.key())
            glow::ShaderFile::reloadAll();
    }

protected:
	glow::ref_ptr<glow::Texture> m_texture;

    glow::ref_ptr<glow::Program> m_computeProgram;
	glow::ref_ptr<glow::Program> m_program;
	
    glow::ref_ptr<glow::VertexArrayObject> m_vao;
	
    glow::ref_ptr<glow::Buffer> m_vertexBuffer;

    unsigned int m_frame;
};


/** This example shows ... .
*/
int main(int argc, char** argv)
{
    glewExperimental = GL_TRUE;

    ContextFormat format;

    Window window;
    window.assign(new EventHandler());

    window.create(format, "Compute Shader Example");
    window.show();
    window.context()->setSwapInterval(Context::NoVerticalSyncronization);

    return Window::run();
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
    glow::Shader* vertexShader = glow::Shader::fromFile(GL_VERTEX_SHADER, "data/computeshader/cstest.vert");
    glow::Shader* fragmentShader = glow::Shader::fromFile(GL_FRAGMENT_SHADER, "data/computeshader/cstest.frag");

	m_program = new glow::Program();
	m_program->attach(vertexShader, fragmentShader);
	m_program->bindFragDataLocation(0, "fragColor");

    glow::Shader* computeShader = glow::Shader::fromFile(GL_COMPUTE_SHADER, "data/computeshader/cstest.comp");

	m_computeProgram = new glow::Program();
	m_computeProgram->attach(computeShader);

	m_program->setUniform("texture", 0);
	m_computeProgram->setUniform("destTex", 0);
}

void EventHandler::createAndSetupGeometry()
{
    auto vertexArray = glow::Vec3Array()
        << glm::vec3( 0, 0, 0)
        << glm::vec3( 1, 0, 0)
        << glm::vec3( 1, 1, 0)
        << glm::vec3( 0, 1, 0);

	m_vao = new glow::VertexArrayObject();

	m_vertexBuffer = new glow::Buffer(GL_ARRAY_BUFFER);
	m_vertexBuffer->setData(vertexArray);

	auto binding = m_vao->binding(0);
	binding->setBuffer(m_vertexBuffer, 0, sizeof(glm::vec3));
	binding->setFormat(3, GL_FLOAT);

	m_vao->enable(m_program->getAttributeLocation("a_vertex"));
}
