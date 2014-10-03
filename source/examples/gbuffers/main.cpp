
#include <glbinding/gl/gl.h>

#include <globjects/Uniform.h>
#include <globjects/Program.h>
#include <globjects/Shader.h>
#include <globjects/Buffer.h>
#include <globjects/Framebuffer.h>
#include <globjects/VertexArray.h>
#include <globjects/DebugMessage.h>
#include <globjects/Texture.h>

#include <globjects/base/File.h>

#include <common/AxisAlignedBoundingBox.h>
#include <common/Icosahedron.h>
#include <common/Camera.h>
#include <common/AbstractCoordinateProvider.h>
#include <common/WorldInHandNavigation.h>
#include <common/StringTemplate.h>
#include <common/ScreenAlignedQuad.h>

#include <common/ContextFormat.h>
#include <common/Context.h>
#include <common/Window.h>
#include <common/WindowEventHandler.h>
#include <common/events.h>

#include <ExampleWindowEventHandler.h>


using namespace gl;
using namespace glm;

class EventHandler : public ExampleWindowEventHandler, AbstractCoordinateProvider
{
public:
    EventHandler()
    : m_camera(vec3(0.f, 1.f, 4.f))
    {
        m_aabb.extend(vec3(-8.f, -1.f, -8.f));
        m_aabb.extend(vec3( 8.f,  1.f,  8.f));

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

        globjects::DebugMessage::enable();

        glClearColor(1.f, 1.f, 1.f, 0.f);


        m_icosahedron = new Icosahedron(2);

        m_sphere = new globjects::Program();

        m_sphere->attach(
            globjects::Shader::fromFile(GL_VERTEX_SHADER,   "data/gbuffers/sphere.vert"),
            globjects::Shader::fromFile(GL_FRAGMENT_SHADER, "data/gbuffers/sphere.frag"));

        m_colorTexture    = globjects::Texture::createDefault(GL_TEXTURE_2D);
        m_depthTexture    = globjects::Texture::createDefault(GL_TEXTURE_2D);
        m_normalTexture   = globjects::Texture::createDefault(GL_TEXTURE_2D);
        m_geometryTexture = globjects::Texture::createDefault(GL_TEXTURE_2D);

        m_sphereFBO = new globjects::Framebuffer;
        m_sphereFBO->attachTexture(GL_COLOR_ATTACHMENT0, m_colorTexture);
        m_sphereFBO->attachTexture(GL_COLOR_ATTACHMENT1, m_normalTexture);
        m_sphereFBO->attachTexture(GL_COLOR_ATTACHMENT2, m_geometryTexture);
        m_sphereFBO->attachTexture(GL_DEPTH_ATTACHMENT,  m_depthTexture);
        m_sphereFBO->setDrawBuffers({ GL_COLOR_ATTACHMENT0, GL_COLOR_ATTACHMENT1, GL_COLOR_ATTACHMENT2 });

        m_postprocessing = new ScreenAlignedQuad(
            globjects::Shader::fromFile(GL_FRAGMENT_SHADER, "data/gbuffers/postprocessing.frag"));
        m_postprocessing->program()->setUniform<GLint>("colorSource",      0);
        m_postprocessing->program()->setUniform<GLint>("normalSource",     1);
        m_postprocessing->program()->setUniform<GLint>("worldCoordSource", 2);
        m_postprocessing->program()->setUniform<GLint>("depthSource",      3);

        m_postprocessedTexture = globjects::Texture::createDefault(GL_TEXTURE_2D);

        m_postprocessingFBO = new globjects::Framebuffer;
        m_postprocessingFBO->attachTexture(GL_COLOR_ATTACHMENT0, m_postprocessedTexture);
        m_postprocessingFBO->setDrawBuffer(GL_COLOR_ATTACHMENT0);

        m_gBufferChoice = new ScreenAlignedQuad(
            globjects::Shader::fromFile(GL_FRAGMENT_SHADER, "data/gbuffers/gbufferchoice.frag"));
        m_gBufferChoice->program()->setUniform<GLint>("postprocessedSource", 0);
        m_gBufferChoice->program()->setUniform<GLint>("colorSource",         1);
        m_gBufferChoice->program()->setUniform<GLint>("normalSource",        2);
        m_gBufferChoice->program()->setUniform<GLint>("worldCoordSource",    3);
        m_gBufferChoice->program()->setUniform<GLint>("depthSource",         4);

        m_camera.setZNear( 1.f);
        m_camera.setZFar (16.f);

        m_gBufferChoice->program()->setUniform<GLfloat>("nearZ", m_camera.zNear());
        m_gBufferChoice->program()->setUniform<GLfloat>("farZ",  m_camera.zFar());

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
        glViewport(0, 0, event.width(), event.height());
        m_camera.setViewport(event.width(), event.height());

        m_postprocessing->program()->setUniform<vec2>("screenSize", vec2(event.size()));

        cameraChanged();

        m_colorTexture->image2D(        0, GL_RGBA8,   event.width(), event.height(), 0, GL_RGBA, GL_UNSIGNED_BYTE, nullptr);
        m_normalTexture->image2D(       0, GL_RGBA16F, event.width(), event.height(), 0, GL_RGBA, GL_FLOAT, nullptr);
        m_geometryTexture->image2D(     0, GL_RGBA16F, event.width(), event.height(), 0, GL_RGBA, GL_FLOAT, nullptr);
        m_depthTexture->image2D(0, GL_DEPTH_COMPONENT, event.width(), event.height(), 0, GL_DEPTH_COMPONENT, GL_FLOAT, nullptr);
        m_postprocessedTexture->image2D(0, GL_RGBA8,   event.width(), event.height(), 0, GL_RGBA, GL_UNSIGNED_BYTE, nullptr);
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

        m_sphereFBO->bind(GL_FRAMEBUFFER);

        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        m_sphere->use();
        m_icosahedron->draw();
        m_sphere->release();

        m_sphereFBO->unbind(GL_FRAMEBUFFER);

        // Postprocessing Pass

        m_postprocessingFBO->bind(GL_FRAMEBUFFER);

        glClear(GL_COLOR_BUFFER_BIT);

        m_colorTexture->bindActive(GL_TEXTURE0);
        m_normalTexture->bindActive(GL_TEXTURE1);
        m_geometryTexture->bindActive(GL_TEXTURE2);
        m_depthTexture->bindActive(GL_TEXTURE3);

        m_postprocessing->draw();

        m_colorTexture->unbindActive(GL_TEXTURE0);
        m_normalTexture->unbindActive(GL_TEXTURE1);
        m_geometryTexture->unbindActive(GL_TEXTURE2);
        m_depthTexture->unbindActive(GL_TEXTURE3);

        m_postprocessingFBO->unbind(GL_FRAMEBUFFER);

        // GBuffer Choice Pass (including blitting)

        // If no FBO is bound to GL_FRAMEBUFFER the default FBO is bound to GL_FRAMEBUFFER

        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        m_postprocessedTexture->bindActive(GL_TEXTURE0);
        m_colorTexture->bindActive(GL_TEXTURE1);
        m_normalTexture->bindActive(GL_TEXTURE2);
        m_geometryTexture->bindActive(GL_TEXTURE3);
        m_depthTexture->bindActive(GL_TEXTURE4);

        m_gBufferChoice->draw();

        m_postprocessedTexture->unbindActive(GL_TEXTURE0);
        m_colorTexture->unbindActive(GL_TEXTURE1);
        m_normalTexture->unbindActive(GL_TEXTURE2);
        m_geometryTexture->unbindActive(GL_TEXTURE3);
        m_depthTexture->unbindActive(GL_TEXTURE4);
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
            globjects::File::reloadAll();
            break;

        case GLFW_KEY_1:
        case GLFW_KEY_2:
        case GLFW_KEY_3:
        case GLFW_KEY_4:
        case GLFW_KEY_5:
            m_gBufferChoice->program()->setUniform<GLint>("choice", GLFW_KEY_1 - 49);
            break;

        case GLFW_KEY_SPACE:
            m_camera.setCenter(vec3( 0.f, 0.f, 0.f));
            m_camera.setEye   (vec3( 0.f, 1.f, 4.f));
            m_camera.setUp    (vec3( 0.f, 1.f, 0.f));
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
        case WorldInHandNavigation::PanInteraction:
            m_nav.panProcess(event.pos());
            event.accept();
            cameraChanged();
            break;

        case WorldInHandNavigation::RotateInteraction:
            m_nav.rotateProcess(event.pos());
            event.accept();
            cameraChanged();
            break;
        case WorldInHandNavigation::NoInteraction:
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
        if (WorldInHandNavigation::NoInteraction != m_nav.mode())
            return;

        m_nav.scaleAtMouse(event.pos(), -event.offset().y * 0.1f);
        event.accept();
        cameraChanged();
    }

    virtual float depthAt(const ivec2 & windowCoordinates) const override
    {
        m_sphereFBO->bind(GL_FRAMEBUFFER);

        float depth = AbstractCoordinateProvider::depthAt(m_camera, GL_DEPTH_COMPONENT, windowCoordinates);

        m_sphereFBO->unbind(GL_FRAMEBUFFER);

        return depth;
    }

    virtual vec3 objAt(const ivec2 & windowCoordinates) const override
    {
        return unproject(m_camera, static_cast<GLenum>(GL_DEPTH_COMPONENT), windowCoordinates);
    }

    virtual vec3 objAt(const ivec2 & windowCoordinates, const float depth) const override
    {
        return unproject(m_camera, depth, windowCoordinates);
    }

    virtual vec3 objAt(
        const ivec2 & windowCoordinates
    ,   const float depth
    ,   const mat4 & viewProjectionInverted) const override
    {
        return unproject(m_camera, viewProjectionInverted, depth, windowCoordinates);
    }

protected:
    globjects::ref_ptr<Icosahedron> m_icosahedron;
    globjects::ref_ptr<globjects::Program> m_sphere;
    globjects::ref_ptr<globjects::Texture> m_colorTexture;
    globjects::ref_ptr<globjects::Texture> m_normalTexture;
    globjects::ref_ptr<globjects::Texture> m_geometryTexture;
    globjects::ref_ptr<globjects::Texture> m_depthTexture;
    globjects::ref_ptr<globjects::Framebuffer> m_sphereFBO;

    globjects::ref_ptr<ScreenAlignedQuad> m_postprocessing;
    globjects::ref_ptr<globjects::Texture> m_postprocessedTexture;
    globjects::ref_ptr<globjects::Framebuffer> m_postprocessingFBO;

    globjects::ref_ptr<ScreenAlignedQuad> m_gBufferChoice;

    Camera m_camera;
    WorldInHandNavigation m_nav;
    ivec2 m_lastMousePos;

    AxisAlignedBoundingBox m_aabb;
};


int main(int /*argc*/, char * /*argv*/[])
{
    globjects::info() << "Usage:";
    globjects::info() << "\t" << "ESC" << "\t\t"        << "Close example";
    globjects::info() << "\t" << "ALT + Enter"          << "\t" << "Toggle fullscreen";
    globjects::info() << "\t" << "F11" << "\t\t"        << "Toggle fullscreen";
    globjects::info() << "\t" << "F5" << "\t\t"         << "Reload shaders";
    globjects::info() << "\t" << "Space" << "\t\t"      << "Reset camera";
    globjects::info() << "\t" << "Left Mouse" << "\t"   << "Pan scene";
    globjects::info() << "\t" << "Right Mouse" << "\t"  << "Rotate scene";
    globjects::info() << "\t" << "Mouse Wheel" << "\t"  << "Zoom scene";

    globjects::info() << "\nSwitch between G-Buffers";
    globjects::info() << "\t" << "1" << "\t" << "Postprocessed";
    globjects::info() << "\t" << "2" << "\t" << "Color";
    globjects::info() << "\t" << "3" << "\t" << "Normal";
    globjects::info() << "\t" << "4" << "\t" << "Geometry";
    globjects::info() << "\t" << "5" << "\t" << "Depth";

    ContextFormat format;
    format.setVersion(3, 2);

    Window window;

    window.setEventHandler(new EventHandler());

    if (!window.create(format, "GBuffers Example"))
        return 1;

    window.context()->setSwapInterval(Context::VerticalSyncronization);
    window.show();

    return MainLoop::run();
}
