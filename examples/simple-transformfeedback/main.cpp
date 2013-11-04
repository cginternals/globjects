
#include <GL/glew.h>

#include <algorithm>
#include <chrono>

#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/transform.hpp>

#include <glow/Error.h>
#include <glow/Uniform.h>
#include <glow/Array.h>
#include <glow/ShaderFile.h>
#include <glow/Program.h>
#include <glow/Shader.h>
#include <glow/Buffer.h>
#include <glow/VertexArrayObject.h>
#include <glow/VertexAttributeBinding.h>
#include <glow/logging.h>
#include <glow/TransformFeedback.h>
#include <glow/Timer.h>

#include <glowwindow/Window.h>
#include <glowwindow/ContextFormat.h>
#include <glowwindow/Context.h>
#include <glowwindow/WindowEventHandler.h>

using namespace glow;

class EventHandler : public WindowEventHandler
{
public:
    EventHandler()
        : m_timer(false, true)
    {
    }

    virtual ~EventHandler()
    {
    }

    void createAndSetupShaders();
	void createAndSetupGeometry();
    void createAndSetupTransformFeedback();

    virtual void initializeEvent(Window & window)
    {
        DebugMessageOutput::enable();

        glClearColor(0.2f, 0.3f, 0.4f, 1.f);

	    createAndSetupShaders();
	    createAndSetupGeometry();
        createAndSetupTransformFeedback();

        m_timer.start();
    }
    
    virtual void resizeEvent(
        Window & window
    ,   const unsigned int width
    ,   const unsigned int height)
    {
    	int side = std::min<int>(width, height);
	    glViewport((width - side) / 2, (height - side) / 2, side, side);

	    m_shaderProgram->setUniform("modelView", glm::mat4());
        m_shaderProgram->setUniform("projection", glm::ortho(-0.4f, 1.4f, -0.4f, 1.4f, 0.f, 1.f));
    }

    virtual void paintEvent(Window & window)
    {
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        glow::Buffer* drawBuffer = m_vertexBuffer1;
        glow::Buffer* writeBuffer = m_vertexBuffer2;

        m_vao->bind();

        m_transformFeedbackProgram->setUniform("deltaT", float(m_timer.elapsed() * float(std::nano::num) / float(std::nano::den)));
        m_timer.reset();

        m_vao->disable(m_shaderProgram->getAttributeLocation("in_position"));
        m_vao->disable(m_shaderProgram->getAttributeLocation("in_color"));
        m_vao->binding(0)->setBuffer(drawBuffer, 0, sizeof(glm::vec4));
        m_vao->binding(0)->setAttribute(m_transformFeedbackProgram->getAttributeLocation("in_position"));
        m_vao->enable(m_transformFeedbackProgram->getAttributeLocation("in_position"));

        writeBuffer->bindBase(GL_TRANSFORM_FEEDBACK_BUFFER, 0);

        glEnable(GL_RASTERIZER_DISCARD);
        m_transformFeedbackProgram->use();
        m_transformFeedback->bind();
        m_transformFeedback->begin(GL_TRIANGLES);
        m_vao->drawArrays(GL_TRIANGLES, 0, 6);
        m_transformFeedback->end();
        m_transformFeedback->unbind();
        glDisable(GL_RASTERIZER_DISCARD);

        m_vao->disable(m_transformFeedbackProgram->getAttributeLocation("in_position"));
        m_vao->binding(0)->setBuffer(writeBuffer, 0, sizeof(glm::vec4));
        m_vao->binding(0)->setAttribute(m_shaderProgram->getAttributeLocation("in_position"));
        m_vao->enable(m_shaderProgram->getAttributeLocation("in_position"));
        m_vao->enable(m_shaderProgram->getAttributeLocation("in_color"));

        m_shaderProgram->use();
        m_transformFeedback->draw(GL_TRIANGLE_STRIP);
        m_shaderProgram->release();

        m_vao->unbind();

        std::swap(m_vertexBuffer1, m_vertexBuffer2);
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
    glow::ref_ptr<glow::Program> m_shaderProgram;
    glow::ref_ptr<glow::Program> m_transformFeedbackProgram;
	
    glow::ref_ptr<glow::VertexArrayObject> m_vao;

    glow::ref_ptr<glow::TransformFeedback> m_transformFeedback;
	
    glow::ref_ptr<glow::Buffer> m_vertexBuffer1;
    glow::ref_ptr<glow::Buffer> m_vertexBuffer2;
    glow::ref_ptr<glow::Buffer> m_colorBuffer;

    glow::Timer m_timer;
};


/** This example shows a simple point which walks a circle using transform feedback.
*/
int main(int argc, char** argv)
{
    glewExperimental = GL_TRUE;

    ContextFormat format;

    Window window;
    window.assign(new EventHandler());

    format.setVersion(4, 0);

    window.create(format, "Simple Transform Feedback Example");
    window.show();
    window.context()->setSwapInterval(Context::NoVerticalSyncronization);

    return Window::run();
}

void EventHandler::createAndSetupShaders()
{
    glow::Shader* vertexShader = glow::Shader::fromFile(GL_VERTEX_SHADER, "data/transformfeedback/simple.vert");
    glow::Shader* fragmentShader = glow::Shader::fromFile(GL_FRAGMENT_SHADER, "data/transformfeedback/simple.frag");
    glow::Shader* transformFeedbackShader = glow::Shader::fromFile(GL_VERTEX_SHADER, "data/transformfeedback/transformfeedback.vert");

	m_shaderProgram = new glow::Program();
    m_shaderProgram->attach(vertexShader, fragmentShader);
	m_shaderProgram->bindFragDataLocation(0, "fragColor");

    m_transformFeedbackProgram = new glow::Program();
    m_transformFeedbackProgram->attach(transformFeedbackShader);
    m_transformFeedbackProgram->setUniform("deltaT", 0.0f);
}

void EventHandler::createAndSetupGeometry()
{
    auto vertexArray = glow::Vec4Array()
        << glm::vec4(0, 0, 0, 1)
        << glm::vec4(1, 0, 0, 1)
        << glm::vec4(0, 1, 0, 1)

        << glm::vec4(1, 0, 0, 1)
        << glm::vec4(0, 1, 0, 1)
        << glm::vec4(1, 1, 0, 1);

    auto colorArray = glow::Vec4Array()
        << glm::vec4(1, 0, 0, 1)
        << glm::vec4(1, 1, 0, 1)
        << glm::vec4(0, 0, 1, 1)

        << glm::vec4(1, 1, 0, 1)
        << glm::vec4(0, 0, 1, 1)
        << glm::vec4(0, 1, 0, 1);

    m_vertexBuffer1 = new glow::Buffer(GL_ARRAY_BUFFER);
    m_vertexBuffer1->setData(vertexArray);
    m_vertexBuffer2 = new glow::Buffer(GL_ARRAY_BUFFER);
    m_vertexBuffer2->setData(vertexArray);
    m_colorBuffer = new glow::Buffer(GL_ARRAY_BUFFER);
    m_colorBuffer->setData(colorArray);

	m_vao = new glow::VertexArrayObject();

    m_vao->binding(0)->setAttribute(m_shaderProgram->getAttributeLocation("in_position"));
    m_vao->binding(0)->setFormat(4, GL_FLOAT);

    m_vao->binding(1)->setAttribute(m_shaderProgram->getAttributeLocation("in_color"));
    m_vao->binding(1)->setBuffer(m_colorBuffer, 0, sizeof(glm::vec4));
    m_vao->binding(1)->setFormat(4, GL_FLOAT);

    m_vao->enable(m_shaderProgram->getAttributeLocation("in_position"));
    m_vao->enable(m_shaderProgram->getAttributeLocation("in_color"));
}

void EventHandler::createAndSetupTransformFeedback()
{
    m_transformFeedback = new glow::TransformFeedback();
    m_transformFeedback->setVaryings(m_transformFeedbackProgram, glow::Array<const char*>{ "next_position" }, GL_INTERLEAVED_ATTRIBS);
}
