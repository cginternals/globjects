
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/transform.hpp>

#include <glow/AutoTimer.h>
#include <glow/Array.h>
#include <glow/VertexArrayObject.h>
#include <glow/Error.h>
#include <glow/Texture.h>
#include <glow/Shader.h>
#include <glow/Program.h>
#include <glow/RawFile.h>
#include <glowwindow/ContextFormat.h>
#include <glowwindow/Context.h>
#include <glowwindow/Window.h>
#include <glowwindow/WindowEventHandler.h>

#include <glow/logging.h>

using namespace glow;

class EventHandler : public WindowEventHandler
{
public:
    EventHandler()
    {
    }

    virtual ~EventHandler()
    {
    }

    void createAndSetupTexture( 
        glow::ref_ptr<Texture> texture
    ,   const std::string & fileName);
    void createAndSetupShaders();
    void createAndSetupGeometry();

    virtual void initializeEvent(Window & window)
    {
        DebugMessageOutput::enable();

        glClearColor(0.2f, 0.3f, 0.4f, 1.f);    

        m_diffuse = new glow::Texture(GL_TEXTURE_2D);
        m_emissive = new glow::Texture(GL_TEXTURE_2D);
        m_normals = new glow::Texture(GL_TEXTURE_2D);
        createAndSetupTexture(m_diffuse, "data/deferred-lighting/diffuse.1280.720.rgb.ub");
        createAndSetupTexture(m_emissive, "data/deferred-lighting/emissive.1280.720.rgb.ub");
        createAndSetupTexture(m_normals, "data/deferred-lighting/normals.1280.720.rgb.ub");
        //createAndSetupTexture(m_diffuse, "data/deferred-lighting/diffuse.1280.720.rgb.ub");
        createAndSetupShaders();
        createAndSetupGeometry();
    }

    virtual void resizeEvent(
        Window & window
    ,   const unsigned int width
    ,   const unsigned int height)
    {
        glViewport(0, 0, width, height);

	    m_program->setUniform("modelView", glm::mat4());
	    m_program->setUniform("projection", glm::ortho(0.f, 1.f, 0.f, 1.f, 0.f, 1.f));
    }

    virtual void paintEvent(Window & window)
    {
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	    m_emissive->bind();
	    m_program->use();

        m_vao->drawArrays(GL_TRIANGLE_FAN, 0, 4);

        m_program->release();
	    m_emissive->unbind();
    }

    virtual void idleEvent(Window & window)
    {
        window.repaint();
    }

protected:
    glow::ref_ptr<glow::Program> m_program;

    glow::ref_ptr<glow::Texture> m_diffuse;
    glow::ref_ptr<glow::Texture> m_emissive;
    glow::ref_ptr<glow::Texture> m_specular;
    glow::ref_ptr<glow::Texture> m_normals;

	glow::ref_ptr<glow::VertexArrayObject> m_vao;
    glow::ref_ptr<glow::Buffer> m_vertexBuffer;
};

/** This example shows how to create a single window, probably in fullscreen
    mode. All window and system events are handled with the event handler.
*/
int main(int argc, char** argv)
{
    ContextFormat format;

    Window window;

    window.assign(new EventHandler());

    window.create(format, "Deferred Lighting");
    window.context()->setSwapInterval(Context::NoVerticalSyncronization);

    window.show();

    return Window::run();
}

void EventHandler::createAndSetupTexture(
    glow::ref_ptr<Texture> texture
,   const std::string & fileName)
{
    glow::RawFile<unsigned char> file(fileName);

    texture->setParameter(GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    texture->setParameter(GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	texture->setParameter(GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	texture->setParameter(GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	texture->setParameter(GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);

    texture->image2D(0, GL_RGB8, 1280, 720, 0, GL_RGB, GL_UNSIGNED_BYTE, file.data());
}

void EventHandler::createAndSetupShaders()
{
    glow::Shader * vertexShader = glow::Shader::fromFile(GL_VERTEX_SHADER, "data/deferred-lighting/deferred.vert");
    glow::Shader * fragmentShader = glow::Shader::fromFile(GL_FRAGMENT_SHADER, "data/deferred-lighting/deferred.frag");

	m_program = new glow::Program();
	m_program->attach(vertexShader, fragmentShader);
	m_program->bindFragDataLocation(0, "fragColor");

	m_program->getUniform<GLint>("texture")->set(0);
}

void EventHandler::createAndSetupGeometry()
{
    auto vertexArray = glow::Vec3Array()
        << glm::vec3(0, 0, 0)
        << glm::vec3(1, 0, 0)
        << glm::vec3(1, 1, 0)
        << glm::vec3(0, 1, 0);

    m_vao = new glow::VertexArrayObject();

    m_vertexBuffer = new glow::Buffer(GL_ARRAY_BUFFER);
    m_vertexBuffer->setData(vertexArray);

    auto binding = m_vao->binding(0);
    binding->setBuffer(m_vertexBuffer, 0, sizeof(glm::vec3));
    binding->setFormat(3, GL_FLOAT);

    m_vao->enable(m_program->getAttributeLocation("a_vertex"));
}

