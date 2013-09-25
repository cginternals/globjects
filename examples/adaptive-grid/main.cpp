
#include <GL/glew.h>

#include <algorithm>
#include <random>

#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/transform.hpp>

#include <glow/Error.h>
#include <glow/Uniform.h>
#include <glow/Array.h>
#include <glow/ShaderFile.h>
#include <glow/Program.h>
#include <glow/VertexArrayObject.h>
#include <glow/logging.h>
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
        Error::setupDebugOutput();
        Error::setChecking(false);

        glClearColor(1.f, 1.f, 1.f, 0.f);

        Shader * vert = Shader::fromFile(GL_VERTEX_SHADER,   "data/adaptive-grid/grid.vert");
        Shader * frag = Shader::fromFile(GL_FRAGMENT_SHADER, "data/adaptive-grid/grid.frag");

        m_program = new Program();
        m_program->attach(vert, frag);
        m_program->bindFragDataLocation(0, "fragColor");

        Array<vec4> points;
        float type;
        int  i = 1;

        type = .2f; // sub gridlines, every 0.125, except every 0.5
        for (float f = -8.f + .125f; f < 8.f; f += .125f)
            if (i++ % 4)
                points << vec4(8.f, 0.f, f, type) << vec4(-8.f, 0.f, f, type) << vec4(f, 0.f, 8.f, type) << vec4(f, 0.f, -8.f, type);
        type = .4f; // grid lines every 1.0 units, offseted by 0.5
        for (float f = -8.f + .5f; f < 8.f; f += 1.f)
            points << vec4(8.f, 0.f, f, type) << vec4(-8.f, 0.f, f, type) << vec4(f, 0.f, 8.f, type) << vec4(f, 0.f, -8.f, type);
        type = .8f; // grid lines every 1.0 units
        for (float f = -8.f + 1.f; f < 8.f; f += 1.f) 
            points << vec4(8.f, 0.f, f, type) << vec4(-8.f, 0.f, f, type) << vec4(f, 0.f, 8.f, type) << vec4(f, 0.f, -8.f, type);


        m_buffer = new Buffer(GL_ARRAY_BUFFER);
        m_buffer->setData(points, GL_STATIC_DRAW);

        m_vao = new VertexArrayObject;

        auto binding = m_vao->binding(0);
        auto a_vertex = m_program->getAttributeLocation("a_vertex");

        binding->setAttribute(a_vertex);
        binding->setBuffer(m_buffer, 0, sizeof(vec4));
        binding->setFormat(4, GL_FLOAT, GL_FALSE, 0);

        m_vao->enable(a_vertex);

    }
    
    virtual void resizeEvent(
        Window & window
    ,   const unsigned int width
    ,   const unsigned int height)
    {
        glViewport(0, 0, width, height);

        if (!m_program)
            return;

        const mat4 projection(perspective(40.f, static_cast<float>(width) / static_cast<float>(height), 0.1f, 8.f));

        const vec3 eye( 0.f, .66f,-2.f);
        const vec3 at ( 0.f, 0.0f, 0.f);
        const vec3 up ( 0.f, 1.0f, 0.f);

        const mat4 view(lookAt(eye, at, up));

        m_program->setUniform("modelView", view);
        m_program->setUniform("projection", projection);
    }

    virtual void paintEvent(Window & window)
    {
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        m_program->use();

        glDisable(GL_DEPTH_TEST);

        m_vao->drawArrays(GL_LINES, 0, 508);

        glEnable(GL_DEPTH_TEST);

        m_program->release();
    }

    virtual void idleEvent(Window & window)
    {
        window.repaint();
    }

    virtual void keyReleaseEvent(
        Window & window
    ,   KeyEvent & event)
    {
        if (KeyEvent::KeyF5 == event.key())
            glow::ShaderFile::reloadAll();
    }

protected:
    //glow::ref_ptr<glow::Texture> m_texture;

    glow::ref_ptr<glow::Program> m_program;

    glow::ref_ptr<glow::VertexArrayObject> m_vao;
    glow::ref_ptr<glow::Buffer> m_buffer;
};


/** This example shows ... .
*/
int main(int argc, char** argv)
{
    glewExperimental = GL_TRUE;

    ContextFormat format;

    Window window;
    window.assign(new EventHandler());

    window.create(format, "Adaptive Grid Example");
    window.show();

    return Window::run();
}

void EventHandler::createAndSetupShaders()
{
	//glow::Shader * vertexShader = glow::Shader::fromFile(GL_VERTEX_SHADER, "data/adaptive-grid/grid.vert");
 //   glow::Shader * fragmentShader = glow::Shader::fromFile(GL_FRAGMENT_SHADER, "data/adaptive-grid/grid.frag");

	//m_shaderProgram = new glow::Program();
	//m_shaderProgram->attach(vertexShader, fragmentShader);
	//m_shaderProgram->bindFragDataLocation(0, "fragColor");

//	m_shaderProgram->getUniform<GLint>("texture")->set(0);
}

void EventHandler::createAndSetupGeometry()
{
	//auto vertices = glow::Array<glm::vec3>()
	//	<< glm::vec3(0,0,0)
	//	<< glm::vec3(1,0,0)
	//	<< glm::vec3(1,1,0)
	//	<< glm::vec3(0,1,0);

	//m_vertexArrayObject = new glow::VertexArrayObject();

	//m_vertexBuffer = new glow::Buffer(GL_ARRAY_BUFFER);
	//m_vertexBuffer->setData(vertices);

	//auto binding1 = m_vertexArrayObject->binding(0);
 //   auto a_vertex = m_shaderProgram->getAttributeLocation("a_vertex");

	//binding1->setAttribute(a_vertex);
	//binding1->setBuffer(m_vertexBuffer, 0, sizeof(glm::vec3));
	//binding1->setFormat(3, GL_FLOAT, GL_FALSE, 0);

	//m_vertexArrayObject->enable(a_vertex);
}
