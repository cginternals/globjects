#include <GL/glew.h>

#include <array>
#include <algorithm>
#include <random>

#include <glow/Error.h>
#include <glow/logging.h>
#include <glow/State.h>
#include <glow/Capability.h>
#include <glow/Texture.h>
#include <glow/Program.h>
#include <glow/Shader.h>
#include <glow/VertexArrayObject.h>
#include <glow/VertexAttributeBinding.h>
#include <glow/Buffer.h>
#include <glow/debugmessageoutput.h>

#include <glowutils/ScreenAlignedQuad.h>
#include <glowutils/global.h>
#include <glowutils/Camera.h>
#include <glowutils/File.h>
#include <glowutils/AxisAlignedBoundingBox.h>
#include <glowutils/AdaptiveGrid.h>
#include <glowutils/AbstractCoordinateProvider.h>
#include <glowutils/WorldInHandNavigation.h>

#include <glowwindow/ContextFormat.h>
#include <glowwindow/Context.h>
#include <glowwindow/Window.h>
#include <glowwindow/WindowEventHandler.h>
#include <glowwindow/events.h>

#include <ExampleWindowEventHandler.h>

using namespace glowwindow;

class EventHandler : public ExampleWindowEventHandler, glowutils::AbstractCoordinateProvider
{
public:
    struct Vertex
    {
        glm::vec3 position;
        glm::vec2 texCoord;
        int side;
    };

    EventHandler()
    {
        m_aabb.extend(glm::vec3(-8.f, -1.f, -8.f));
        m_aabb.extend(glm::vec3(8.f, 1.f, 8.f));

        m_camera = glowutils::Camera(glm::vec3(0.0, 1.0, -1.0)*2.f, glm::vec3(), glm::vec3(0.0, 1.0, 0.0));

        m_nav.setCamera(&m_camera);
        m_nav.setCoordinateProvider(this);
        m_nav.setBoundaryHint(m_aabb);
    }

    virtual ~EventHandler()
    {
    }

    void createAndSetupTexture();
	void createAndSetupGeometry();

    virtual void initialize(Window & window) override
    {
        glow::debugmessageoutput::enable();

        glow::State state;
        state.enable(GL_CULL_FACE);
        state.clearColor(0.2f, 0.3f, 0.4f, 1.f);

        m_program = new glow::Program;
        m_program->attach(
            glowutils::createShaderFromFile(GL_VERTEX_SHADER, "data/bindless-textures/shader.vert"),
            glowutils::createShaderFromFile(GL_FRAGMENT_SHADER, "data/bindless-textures/shader.frag")
        );

        createAndSetupTexture();
	    createAndSetupGeometry();

        window.addTimer(0, 0);
    }
    
    virtual void framebufferResizeEvent(ResizeEvent & event) override
    {
        int width = event.width();
        int height = event.height();
        //int side = std::min<int>(width, height);

        glViewport(0, 0, width, height);
        CheckGLError();

        m_camera.setViewport(width, height);
    }

    virtual void paintEvent(PaintEvent &) override
    {
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        CheckGLError();

        m_program->setUniform("projection", m_camera.viewProjection());

        m_program->use();
        m_vao->drawArrays(GL_TRIANGLES, 0, (int)m_vertices.size());
        m_program->release();
    }

    virtual void timerEvent(TimerEvent & event) override
    {        
        event.window()->repaint();
    }

    virtual void keyPressEvent(KeyEvent & event) override
    {
        if (event.key()==GLFW_KEY_F5)
            glowutils::File::reloadAll();
        else if (event.key()==GLFW_KEY_SPACE)
        {
            m_nav.reset();
            event.window()->repaint();
        }
    }

    virtual void mousePressEvent(MouseEvent & event) override
    {
        switch (event.button())
        {
            case GLFW_MOUSE_BUTTON_LEFT:
                m_nav.panBegin(event.pos());
                event.accept();
                break;

            case GLFW_MOUSE_BUTTON_RIGHT:
                m_nav.rotateBegin(event.pos());
                event.accept();
                break;
        }
    }
    virtual void mouseMoveEvent(MouseEvent & event) override
    {
        switch (m_nav.mode())
        {
            case glowutils::WorldInHandNavigation::PanInteraction:
                m_nav.panProcess(event.pos());
                event.accept();
                break;

            case glowutils::WorldInHandNavigation::RotateInteraction:
                m_nav.rotateProcess(event.pos());
                event.accept();

            default:
                break;
        }
    }
    virtual void mouseReleaseEvent(MouseEvent & event) override
    {
        switch (event.button())
        {
            case GLFW_MOUSE_BUTTON_LEFT:
                m_nav.panEnd();
                event.accept();
                break;

            case GLFW_MOUSE_BUTTON_RIGHT:
                m_nav.rotateEnd();
                event.accept();
                break;
        }
    }

    virtual void scrollEvent(ScrollEvent & event) override
    {
        if (glowutils::WorldInHandNavigation::NoInteraction != m_nav.mode())
            return;

        m_nav.scaleAtMouse(event.pos(), -event.offset().y * 0.1f);
        event.accept();
    }

    virtual float depthAt(const glm::ivec2 & windowCoordinates) override
    {
        return AbstractCoordinateProvider::depthAt(m_camera, GL_DEPTH_COMPONENT, windowCoordinates);
    }

    virtual glm::vec3 objAt(const glm::ivec2 & windowCoordinates) override
    {
        return unproject(m_camera, static_cast<GLenum>(GL_DEPTH_COMPONENT), windowCoordinates);
    }

    virtual glm::vec3 objAt(const glm::ivec2 & windowCoordinates, const float depth) override
    {
        return unproject(m_camera, depth, windowCoordinates);
    }

    virtual glm::vec3 objAt(
        const glm::ivec2 & windowCoordinates
    ,   const float depth
    ,   const glm::mat4 & viewProjectionInverted) override
    {
        return unproject(m_camera, viewProjectionInverted, depth, windowCoordinates);
    }

protected:
    glowutils::Camera m_camera;

    glowutils::WorldInHandNavigation m_nav;
    glowutils::AxisAlignedBoundingBox m_aabb;

    std::array<glow::ref_ptr<glow::Texture>, 4> m_textures;
    glow::ref_ptr<glow::VertexArrayObject> m_vao;
    glow::ref_ptr<glow::Buffer> m_vbo;
    glow::ref_ptr<glow::Program> m_program;
    std::vector<Vertex> m_vertices;
};


/** This example shows ... .
*/
int main(int /*argc*/, char* /*argv*/[])
{
    ContextFormat format;
    format.setVersion(3, 0);

    Window window;

    window.setEventHandler(new EventHandler());

    if (window.create(format, "Bindless Texture Example"))
    {
        window.context()->setSwapInterval(Context::NoVerticalSyncronization);

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
    static const int w = 256;
    static const int h = 256;
	unsigned char data[w * h * 4];

    std::random_device rd;
    std::mt19937 generator(rd());

    std::vector<glow::TextureHandle> handles(m_textures.size());

    for (unsigned i = 0; i < m_textures.size(); ++i)
    {
        glow::Texture* texture = new glow::Texture(GL_TEXTURE_2D);

        texture->setParameter(GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        texture->setParameter(GL_TEXTURE_MAG_FILTER, GL_LINEAR);

        texture->setParameter(GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
        texture->setParameter(GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
        texture->setParameter(GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);

        for (unsigned j = 0; j < w*h*4; ++j)
        {
            data[j] = static_cast<unsigned char>(255 - static_cast<unsigned char>(generator() * 255));

            if (j%4==i)
                data[j] = 0;
        }

        texture->image2D(0, GL_RGBA8, w, h, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);

        m_textures[i] = texture;

        glow::TextureHandle handle = texture->makeResident();

        handles[i] = handle;
    }

    m_program->setUniform("textures", handles);
}

void EventHandler::createAndSetupGeometry()
{
    std::array<glm::vec3, 8> points = {
        glm::vec3(glm::sin(glm::radians(0.f)), 0.0, glm::cos(glm::radians(0.f))),
        glm::vec3(glm::sin(glm::radians(120.f)), 0.0, glm::cos(glm::radians(120.f))),
        glm::vec3(glm::sin(glm::radians(240.f)), 0.0, glm::cos(glm::radians(240.f))),
        glm::vec3(0.0, 1.f, 0.0)
    };

    m_vertices = {
        Vertex{ points[0], glm::vec2(0.0, 0.0), 0 },
        Vertex{ points[2], glm::vec2(1.0, 0.0), 0 },
        Vertex{ points[1], glm::vec2(0.5, 1.0), 0 },

        Vertex{ points[1], glm::vec2(0.0, 0.0), 1 },
        Vertex{ points[2], glm::vec2(1.0, 0.0), 1 },
        Vertex{ points[3], glm::vec2(0.5, 1.0), 1 },

        Vertex{ points[0], glm::vec2(0.0, 0.0), 2 },
        Vertex{ points[3], glm::vec2(1.0, 0.0), 2 },
        Vertex{ points[2], glm::vec2(0.5, 1.0), 2 },

        Vertex{ points[0], glm::vec2(0.0, 0.0), 3 },
        Vertex{ points[1], glm::vec2(1.0, 0.0), 3 },
        Vertex{ points[3], glm::vec2(0.5, 1.0), 3 }
    };

    m_vbo = new glow::Buffer(GL_ARRAY_BUFFER);
    m_vbo->setData(m_vertices);

    m_vao = new glow::VertexArrayObject;

    m_vao->binding(0)->setAttribute(0);
    m_vao->binding(0)->setBuffer(m_vbo, 0, sizeof(Vertex));
    m_vao->binding(0)->setFormat(3, GL_FLOAT, GL_FALSE, offsetof(Vertex, position));

    m_vao->binding(1)->setAttribute(1);
    m_vao->binding(1)->setBuffer(m_vbo, 0, sizeof(Vertex));
    m_vao->binding(1)->setFormat(2, GL_FLOAT, GL_FALSE, offsetof(Vertex, texCoord));

    m_vao->binding(2)->setAttribute(2);
    m_vao->binding(2)->setBuffer(m_vbo, 0, sizeof(Vertex));
    m_vao->binding(2)->setIFormat(1, GL_INT, offsetof(Vertex, side));

    m_vao->enable(0);
    m_vao->enable(1);
    m_vao->enable(2);
}
