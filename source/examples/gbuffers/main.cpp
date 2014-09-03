#include <glbinding/gl/gl.h>

#include <globjects-base/File.h>

#include <globjects/Uniform.h>
#include <globjects/Program.h>
#include <globjects/Shader.h>
#include <globjects/Buffer.h>
#include <globjects/Framebuffer.h>
#include <globjects/VertexArray.h>
#include <globjects/DebugMessage.h>
#include <globjects/Texture.h>

#include <globjects-utils/AxisAlignedBoundingBox.h>
#include <globjects-utils/Icosahedron.h>
#include <globjects-utils/Camera.h>
#include <globjects-utils/AbstractCoordinateProvider.h>
#include <globjects-utils/WorldInHandNavigation.h>
#include <globjects-utils/globjects-utils.h>
#include <globjects-utils/StringTemplate.h>
#include <globjects-utils/ScreenAlignedQuad.h>

#include <common/ContextFormat.h>
#include <common/Context.h>
#include <common/Window.h>
#include <common/WindowEventHandler.h>
#include <common/events.h>

#include <ExampleWindowEventHandler.h>


using namespace glm;


class EventHandler : public ExampleWindowEventHandler, gloutils::AbstractCoordinateProvider
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

        glo::DebugMessage::enable();

        gl::glClearColor(1.0f, 1.0f, 1.0f, 0.f);

        auto vertexShaderSource = new gloutils::StringTemplate(new glo::File("data/gbuffers/sphere.vert"));
        auto fragmentShaderSource = new gloutils::StringTemplate(new glo::File("data/gbuffers/sphere.frag"));
        auto postprocessingSource = new gloutils::StringTemplate(new glo::File("data/gbuffers/postprocessing.frag"));
        auto gBufferChoiceSource = new gloutils::StringTemplate(new glo::File("data/gbuffers/gbufferchoice.frag"));

#ifdef MAC_OS
        vertexShaderSource->replace("#version 140", "#version 150");
        fragmentShaderSource->replace("#version 140", "#version 150");
        postprocessingSource->replace("#version 140", "#version 150");
        gBufferChoiceSource->replace("#version 140", "#version 150");
#endif

        m_icosahedron = new gloutils::Icosahedron(2);

        m_sphere = new glo::Program();

        m_sphere->attach(
            new glo::Shader(gl::GL_VERTEX_SHADER, vertexShaderSource),
            new glo::Shader(gl::GL_FRAGMENT_SHADER, fragmentShaderSource)
        );

        m_colorTexture = glo::Texture::createDefault(gl::GL_TEXTURE_2D);
        m_depthTexture = glo::Texture::createDefault(gl::GL_TEXTURE_2D);
        m_normalTexture = glo::Texture::createDefault(gl::GL_TEXTURE_2D);
        m_geometryTexture = glo::Texture::createDefault(gl::GL_TEXTURE_2D);

        m_sphereFBO = new glo::Framebuffer;
        m_sphereFBO->attachTexture(gl::GL_COLOR_ATTACHMENT0, m_colorTexture);
        m_sphereFBO->attachTexture(gl::GL_COLOR_ATTACHMENT1, m_normalTexture);
        m_sphereFBO->attachTexture(gl::GL_COLOR_ATTACHMENT2, m_geometryTexture);
        m_sphereFBO->attachTexture(gl::GL_DEPTH_ATTACHMENT, m_depthTexture);
        m_sphereFBO->setDrawBuffers({ gl::GL_COLOR_ATTACHMENT0, gl::GL_COLOR_ATTACHMENT1, gl::GL_COLOR_ATTACHMENT2 });

        m_postprocessing = new gloutils::ScreenAlignedQuad(new glo::Shader(gl::GL_FRAGMENT_SHADER, postprocessingSource));
        m_postprocessing->program()->setUniform<gl::GLint>("colorSource", 0);
        m_postprocessing->program()->setUniform<gl::GLint>("normalSource", 1);
        m_postprocessing->program()->setUniform<gl::GLint>("worldCoordSource", 2);
        m_postprocessing->program()->setUniform<gl::GLint>("depthSource", 3);

        m_postprocessedTexture = glo::Texture::createDefault(gl::GL_TEXTURE_2D);

        m_postprocessingFBO = new glo::Framebuffer;
        m_postprocessingFBO->attachTexture(gl::GL_COLOR_ATTACHMENT0, m_postprocessedTexture);
        m_postprocessingFBO->setDrawBuffer(gl::GL_COLOR_ATTACHMENT0);

        m_gBufferChoice = new gloutils::ScreenAlignedQuad(new glo::Shader(gl::GL_FRAGMENT_SHADER, gBufferChoiceSource));
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

        m_sphereFBO->bind(gl::GL_FRAMEBUFFER);

        gl::glClear(gl::GL_COLOR_BUFFER_BIT | gl::GL_DEPTH_BUFFER_BIT);

        m_sphere->use();
        m_icosahedron->draw();
        m_sphere->release();

        m_sphereFBO->unbind(gl::GL_FRAMEBUFFER);

        // Postprocessing Pass

        m_postprocessingFBO->bind(gl::GL_FRAMEBUFFER);

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

        m_postprocessingFBO->unbind(gl::GL_FRAMEBUFFER);

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
            glo::File::reloadAll();
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
        case gloutils::WorldInHandNavigation::PanInteraction:
            m_nav.panProcess(event.pos());
            event.accept();
            cameraChanged();
            break;

        case gloutils::WorldInHandNavigation::RotateInteraction:
            m_nav.rotateProcess(event.pos());
            event.accept();
            cameraChanged();
            break;
        case gloutils::WorldInHandNavigation::NoInteraction:
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
        if (gloutils::WorldInHandNavigation::NoInteraction != m_nav.mode())
            return;

        m_nav.scaleAtMouse(event.pos(), -event.offset().y * 0.1f);
        event.accept();
        cameraChanged();
    }

    virtual float depthAt(const ivec2 & windowCoordinates) const override
    {
        m_sphereFBO->bind(gl::GL_FRAMEBUFFER);

        float depth = AbstractCoordinateProvider::depthAt(m_camera, gl::GL_DEPTH_COMPONENT, windowCoordinates);

        m_sphereFBO->unbind(gl::GL_FRAMEBUFFER);

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
    glo::ref_ptr<gloutils::Icosahedron> m_icosahedron;
    glo::ref_ptr<glo::Program> m_sphere;
    glo::ref_ptr<glo::Texture> m_colorTexture;
    glo::ref_ptr<glo::Texture> m_normalTexture;
    glo::ref_ptr<glo::Texture> m_geometryTexture;
    glo::ref_ptr<glo::Texture> m_depthTexture;
    glo::ref_ptr<glo::Framebuffer> m_sphereFBO;

    glo::ref_ptr<gloutils::ScreenAlignedQuad> m_postprocessing;
    glo::ref_ptr<glo::Texture> m_postprocessedTexture;
    glo::ref_ptr<glo::Framebuffer> m_postprocessingFBO;

    glo::ref_ptr<gloutils::ScreenAlignedQuad> m_gBufferChoice;

    gloutils::Camera m_camera;
    gloutils::WorldInHandNavigation m_nav;
    glm::ivec2 m_lastMousePos;

    gloutils::AxisAlignedBoundingBox m_aabb;
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
    glo::info() << "\t" << "Space" << "\t\t" << "Reset camera";
    glo::info() << "\t" << "Left Mouse" << "\t" << "Pan scene";
    glo::info() << "\t" << "Right Mouse" << "\t" << "Rotate scene";
    glo::info() << "\t" << "Mouse Wheel" << "\t" << "Zoom scene";

    glo::info() << "\nSwitch between G-Buffers";
    glo::info() << "\t" << "1" << "\t" << "Postprocessed";
    glo::info() << "\t" << "2" << "\t" << "Color";
    glo::info() << "\t" << "3" << "\t" << "Normal";
    glo::info() << "\t" << "4" << "\t" << "Geometry";
    glo::info() << "\t" << "5" << "\t" << "Depth";


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
