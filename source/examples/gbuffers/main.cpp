#include <glowbase/File.h>

#include <glow/Uniform.h>
#include <glow/Program.h>
#include <glow/Shader.h>
#include <glow/Buffer.h>
#include <glow/FrameBufferObject.h>
#include <glow/VertexArrayObject.h>
#include <glow/debugmessageoutput.h>
#include <glow/Texture.h>

#include <glowutils/AxisAlignedBoundingBox.h>
#include <glowutils/Icosahedron.h>
#include <glowutils/Camera.h>
#include <glowutils/AbstractCoordinateProvider.h>
#include <glowutils/WorldInHandNavigation.h>
#include <glowutils/glowutils.h>
#include <glowutils/StringTemplate.h>
#include <glowutils/ScreenAlignedQuad.h>

#include <glowwindow/ContextFormat.h>
#include <glowwindow/Context.h>
#include <glowwindow/Window.h>
#include <glowwindow/WindowEventHandler.h>
#include <glowwindow/events.h>

#include <ExampleWindowEventHandler.h>

using namespace glowwindow;
using namespace glm;


class EventHandler : public ExampleWindowEventHandler, glowutils::AbstractCoordinateProvider
{
public:
    EventHandler()
    : m_camera(vec3(0.f, 1.f, 4.0f))
    {
        m_aabb.extend(vec3(-8.f, -1.f, -8.f));
        m_aabb.extend(vec3(8.f, 1.f, 8.f));

        m_nav.setCamera(&m_camera);
        m_nav.setCoordinateProvider(this);
        m_nav.setBoundaryHint(m_aabb);
    }

    virtual ~EventHandler()
    {
    }

    virtual void initialize(Window & window) override
    {
        ExampleWindowEventHandler::initialize(window);

        glow::debugmessageoutput::enable();

        gl::glClearColor(1.0f, 1.0f, 1.0f, 0.f);

        auto vertexShaderSource = new glowutils::StringTemplate(new glow::File("data/gbuffers/sphere.vert"));
        auto fragmentShaderSource = new glowutils::StringTemplate(new glow::File("data/gbuffers/sphere.frag"));
        auto postprocessingSource = new glowutils::StringTemplate(new glow::File("data/gbuffers/postprocessing.frag"));
        auto gBufferChoiceSource = new glowutils::StringTemplate(new glow::File("data/gbuffers/gbufferchoice.frag"));

#ifdef MAC_OS
        vertexShaderSource->replace("#version 140", "#version 150");
        fragmentShaderSource->replace("#version 140", "#version 150");
        postprocessingSource->replace("#version 140", "#version 150");
        gBufferChoiceSource->replace("#version 140", "#version 150");
#endif

        m_icosahedron = new glowutils::Icosahedron(2);

        m_sphere = new glow::Program();

        m_sphere->attach(
            new glow::Shader(gl::GL_VERTEX_SHADER, vertexShaderSource),
            new glow::Shader(gl::GL_FRAGMENT_SHADER, fragmentShaderSource)
        );

        m_colorTexture = glow::Texture::createDefault(gl::GL_TEXTURE_2D);
        m_depthTexture = glow::Texture::createDefault(gl::GL_TEXTURE_2D);
        m_normalTexture = glow::Texture::createDefault(gl::GL_TEXTURE_2D);
        m_geometryTexture = glow::Texture::createDefault(gl::GL_TEXTURE_2D);

        m_sphereFBO = new glow::FrameBufferObject;
        m_sphereFBO->attachTexture(gl::GL_COLOR_ATTACHMENT0, m_colorTexture);
        m_sphereFBO->attachTexture(gl::GL_COLOR_ATTACHMENT1, m_normalTexture);
        m_sphereFBO->attachTexture(gl::GL_COLOR_ATTACHMENT2, m_geometryTexture);
        m_sphereFBO->attachTexture(gl::GL_DEPTH_ATTACHMENT, m_depthTexture);
        m_sphereFBO->setDrawBuffers({ gl::GL_COLOR_ATTACHMENT0, gl::GL_COLOR_ATTACHMENT1, gl::GL_COLOR_ATTACHMENT2 });

        m_postprocessing = new glowutils::ScreenAlignedQuad(new glow::Shader(gl::GL_FRAGMENT_SHADER, postprocessingSource));
        m_postprocessing->program()->setUniform<gl::GLint>("colorSource", 0);
        m_postprocessing->program()->setUniform<gl::GLint>("normalSource", 1);
        m_postprocessing->program()->setUniform<gl::GLint>("worldCoordSource", 2);
        m_postprocessing->program()->setUniform<gl::GLint>("depthSource", 3);

        m_postprocessedTexture = glow::Texture::createDefault(gl::GL_TEXTURE_2D);

        m_postprocessingFBO = new glow::FrameBufferObject;
        m_postprocessingFBO->attachTexture(gl::GL_COLOR_ATTACHMENT0, m_postprocessedTexture);
        m_postprocessingFBO->setDrawBuffer(gl::GL_COLOR_ATTACHMENT0);

        m_gBufferChoice = new glowutils::ScreenAlignedQuad(new glow::Shader(gl::GL_FRAGMENT_SHADER, gBufferChoiceSource));
        m_gBufferChoice->program()->setUniform<gl::GLint>("postprocessedSource", 0);
        m_gBufferChoice->program()->setUniform<gl::GLint>("colorSource", 1);
        m_gBufferChoice->program()->setUniform<gl::GLint>("normalSource", 2);
        m_gBufferChoice->program()->setUniform<gl::GLint>("worldCoordSource", 3);
        m_gBufferChoice->program()->setUniform<gl::GLint>("depthSource", 4);
        
        m_camera.setZNear(0.1f);
        m_camera.setZFar(1024.f);

        m_gBufferChoice->program()->setUniform<gl::GLfloat>("nearZ", m_camera.zNear());
        m_gBufferChoice->program()->setUniform<gl::GLfloat>("farZ", m_camera.zFar());

        window.addTimer(0, 0, false);

        cameraChanged();
    }

    virtual void finalize(Window &) override
    {
        m_sphere = nullptr;
        m_icosahedron = nullptr;
    }

    virtual void framebufferResizeEvent(ResizeEvent & event) override
    {
        gl::glViewport(0, 0, event.width(), event.height());

        m_postprocessing->program()->setUniform<glm::vec2>("screenSize", glm::vec2(event.size()));

        m_camera.setViewport(event.width(), event.height());

        cameraChanged();

        m_colorTexture->image2D(0, gl::GL_RGBA8, event.width(), event.height(), 0, gl::GL_RGBA, gl::GL_UNSIGNED_BYTE, nullptr);
        m_normalTexture->image2D(0, gl::GL_RGBA16F, event.width(), event.height(), 0, gl::GL_RGBA, gl::GL_FLOAT, nullptr);
        m_geometryTexture->image2D(0, gl::GL_RGBA16F, event.width(), event.height(), 0, gl::GL_RGBA, gl::GL_FLOAT, nullptr);
        m_depthTexture->image2D(0, gl::GL_DEPTH_COMPONENT, event.width(), event.height(), 0, gl::GL_DEPTH_COMPONENT, gl::GL_FLOAT, nullptr);
        m_postprocessedTexture->image2D(0, gl::GL_RGBA8, event.width(), event.height(), 0, gl::GL_RGBA, gl::GL_UNSIGNED_BYTE, nullptr);
    }

    void cameraChanged()
    {
        m_sphere->setUniform("transform", m_camera.viewProjection());
        m_sphere->setUniform("modelView", m_camera.view());
        m_sphere->setUniform("normalMatrix", m_camera.normal());
    }

    virtual void paintEvent(PaintEvent &) override
    {
        // Sphere Pass

        m_sphereFBO->bind();

        gl::glClear(gl::GL_COLOR_BUFFER_BIT | gl::GL_DEPTH_BUFFER_BIT);

        m_sphere->use();
        m_icosahedron->draw();
        m_sphere->release();

        m_sphereFBO->unbind();

        // Postprocessing Pass

        m_postprocessingFBO->bind();

        gl::glClear(gl::GL_COLOR_BUFFER_BIT);

        m_colorTexture->bindActive(gl::GL_TEXTURE0);
        m_normalTexture->bindActive(gl::GL_TEXTURE1);
        m_geometryTexture->bindActive(gl::GL_TEXTURE2);
        m_depthTexture->bindActive(gl::GL_TEXTURE3);

        m_postprocessing->draw();

        m_colorTexture->unbindActive(gl::GL_TEXTURE0);
        m_normalTexture->unbindActive(gl::GL_TEXTURE1);
        m_geometryTexture->unbindActive(gl::GL_TEXTURE2);
        m_depthTexture->unbindActive(gl::GL_TEXTURE3);

        m_postprocessingFBO->unbind();

        // GBuffer Choice Pass (including blitting)

        // If no FBO is bound to GL_FRAMEBUFFER the default FBO is bound to GL_FRAMEBUFFER

        gl::glClear(gl::GL_COLOR_BUFFER_BIT | gl::GL_DEPTH_BUFFER_BIT);

        m_postprocessedTexture->bindActive(gl::GL_TEXTURE0);
        m_colorTexture->bindActive(gl::GL_TEXTURE1);
        m_normalTexture->bindActive(gl::GL_TEXTURE2);
        m_geometryTexture->bindActive(gl::GL_TEXTURE3);
        m_depthTexture->bindActive(gl::GL_TEXTURE4);

        m_gBufferChoice->draw();

        m_postprocessedTexture->unbindActive(gl::GL_TEXTURE0);
        m_colorTexture->unbindActive(gl::GL_TEXTURE1);
        m_normalTexture->unbindActive(gl::GL_TEXTURE2);
        m_geometryTexture->unbindActive(gl::GL_TEXTURE3);
        m_depthTexture->unbindActive(gl::GL_TEXTURE4);
    }

    virtual void timerEvent(TimerEvent & event) override
    {
        event.window()->repaint();
    }

    virtual void keyPressEvent(KeyEvent & event) override
    {
        switch (event.key())
        {
        case GLFW_KEY_F5:
            glow::File::reloadAll();
            break;
        case GLFW_KEY_1:
            m_gBufferChoice->program()->setUniform<gl::GLint>("choice", 0);
            break;
        case GLFW_KEY_2:
            m_gBufferChoice->program()->setUniform<gl::GLint>("choice", 1);
            break;
        case GLFW_KEY_3:
            m_gBufferChoice->program()->setUniform<gl::GLint>("choice", 2);
            break;
        case GLFW_KEY_4:
            m_gBufferChoice->program()->setUniform<gl::GLint>("choice", 3);
            break;
        case GLFW_KEY_5:
            m_gBufferChoice->program()->setUniform<gl::GLint>("choice", 4);
            break;
        case GLFW_KEY_SPACE:
            m_camera.setCenter(vec3());
            m_camera.setEye(vec3(0.f, 1.f, 4.0f));
            m_camera.setUp(vec3(0,1,0));
            cameraChanged();
            break;
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
            cameraChanged();
            break;

        case glowutils::WorldInHandNavigation::RotateInteraction:
            m_nav.rotateProcess(event.pos());
            event.accept();
            cameraChanged();
            break;
        case glowutils::WorldInHandNavigation::NoInteraction:
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
        cameraChanged();
    }

    virtual float depthAt(const ivec2 & windowCoordinates) const override
    {
        m_sphereFBO->bind();

        float depth = AbstractCoordinateProvider::depthAt(m_camera, gl::GL_DEPTH_COMPONENT, windowCoordinates);

        m_sphereFBO->unbind();

        return depth;
    }

    virtual vec3 objAt(const ivec2 & windowCoordinates) const override
    {
        return unproject(m_camera, static_cast<gl::GLenum>(gl::GL_DEPTH_COMPONENT), windowCoordinates);
    }

    virtual vec3 objAt(const ivec2 & windowCoordinates, const float depth) const override
    {
        return unproject(m_camera, depth, windowCoordinates);
    }

    virtual glm::vec3 objAt(
        const ivec2 & windowCoordinates
    ,   const float depth
    ,   const mat4 & viewProjectionInverted) const override
    {
        return unproject(m_camera, viewProjectionInverted, depth, windowCoordinates);
    }

protected:
    glow::ref_ptr<glowutils::Icosahedron> m_icosahedron;
    glow::ref_ptr<glow::Program> m_sphere;
    glow::ref_ptr<glow::Texture> m_colorTexture;
    glow::ref_ptr<glow::Texture> m_normalTexture;
    glow::ref_ptr<glow::Texture> m_geometryTexture;
    glow::ref_ptr<glow::Texture> m_depthTexture;
    glow::ref_ptr<glow::FrameBufferObject> m_sphereFBO;

    glow::ref_ptr<glowutils::ScreenAlignedQuad> m_postprocessing;
    glow::ref_ptr<glow::Texture> m_postprocessedTexture;
    glow::ref_ptr<glow::FrameBufferObject> m_postprocessingFBO;

    glow::ref_ptr<glowutils::ScreenAlignedQuad> m_gBufferChoice;

    glowutils::Camera m_camera;
    glowutils::WorldInHandNavigation m_nav;
    glm::ivec2 m_lastMousePos;

    glowutils::AxisAlignedBoundingBox m_aabb;
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
    glow::info() << "\t" << "Space" << "\t\t" << "Reset camera";
    glow::info() << "\t" << "Left Mouse" << "\t" << "Pan scene";
    glow::info() << "\t" << "Right Mouse" << "\t" << "Rotate scene";
    glow::info() << "\t" << "Mouse Wheel" << "\t" << "Zoom scene";

    glow::info() << "\nSwitch between G-Buffers";
    glow::info() << "\t" << "1" << "\t" << "Postprocessed";
    glow::info() << "\t" << "2" << "\t" << "Color";
    glow::info() << "\t" << "3" << "\t" << "Normal";
    glow::info() << "\t" << "4" << "\t" << "Geometry";
    glow::info() << "\t" << "5" << "\t" << "Depth";


    ContextFormat format;
    format.setVersion(3, 1);

    Window window;

    window.setEventHandler(new EventHandler());

    if (window.create(format, "GBuffers Example"))
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
