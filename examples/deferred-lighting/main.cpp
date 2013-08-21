
#include <GL/glew.h>

#include <glow/AutoTimer.h>
#include <glow/Window.h>
#include <glow/ContextFormat.h>
#include <glow/Context.h>
#include <glow/Error.h>
#include <glow/Texture.h>
#include <glow/RawFile.h>
#include <glow/WindowEventHandler.h>

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

    void createAndSetupTexture();
    void createAndSetupShaders();
    void createAndSetupGeometry();

    virtual void initializeEvent(Window & window)
    {
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
        glViewport(0, 0, width, height);
    }

    virtual void paintEvent(Window & window)
    {
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    }

    virtual void idleEvent(Window & window)
    {
        window.repaint();
    }

protected:
    glow::ref_ptr<glow::Texture> m_diffuse;
    glow::ref_ptr<glow::Texture> m_emissive;
    glow::ref_ptr<glow::Texture> m_specular;
    glow::ref_ptr<glow::Texture> m_normals;

};

/** This example shows how to create a single window, probably in fullscreen
    mode. All window and system events are handled with the event handler.
*/
int main(int argc, char** argv)
{
    glewExperimental = GL_TRUE;

    ContextFormat format;
    EventHandler handler;

    Window window;

    window.attach(&handler);

    window.create(format, "Deferred Lighting");
    window.context()->setSwapInterval(Context::NoVerticalSyncronization);

    window.show();

    return Window::run();
}

void EventHandler::createAndSetupTexture()
{
    glow::RawFile<unsigned char> file("data/deferred-lighting/deferred.vert");

    //m_texture = new glow::Texture(GL_TEXTURE_2D);

    //m_texture->setParameter(GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    //m_texture->setParameter(GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    //m_texture->image2D(0, GL_R32F, 512, 512, 0, GL_RED, GL_FLOAT, nullptr);
    //m_texture->bindImageTexture(0, 0, GL_FALSE, 0, GL_WRITE_ONLY, GL_R32F);
}

void EventHandler::createAndSetupShaders()
{
    /*glow::Shader* vertexShader = glow::Shader::fromFile(GL_VERTEX_SHADER, "data/computeshader/cstest.vert");
    glow::Shader* fragmentShader = glow::Shader::fromFile(GL_FRAGMENT_SHADER, "data/computeshader/cstest.frag");

    m_shaderProgram = new glow::Program();
    m_shaderProgram->attach(vertexShader, fragmentShader);
    m_shaderProgram->bindFragDataLocation(0, "fragColor");

    glow::Shader* computeShader = glow::Shader::fromFile(GL_COMPUTE_SHADER, "data/computeshader/cstest.comp");

    m_computeProgram = new glow::Program();
    m_computeProgram->attach(computeShader);

    m_shaderProgram->setUniform("texture", 0);
    m_computeProgram->setUniform("destTex", 0);*/
}

void EventHandler::createAndSetupGeometry()
{
    //auto vertexArray = glow::Vec3Array()
    //    << glm::vec3(0, 0, 0)
    //    << glm::vec3(1, 0, 0)
    //    << glm::vec3(1, 1, 0)
    //    << glm::vec3(0, 1, 0);

    //m_vao = new glow::VertexArrayObject();

    //m_vertexBuffer = new glow::Buffer(GL_ARRAY_BUFFER);
    //m_vertexBuffer->setData(vertexArray);

    //auto binding = m_vao->binding(0);
    //binding->setBuffer(m_vertexBuffer, 0, sizeof(glm::vec3));
    //binding->setFormat(3, GL_FLOAT);

    //m_vao->enable(m_shaderProgram->getAttributeLocation("a_vertex"));
}

