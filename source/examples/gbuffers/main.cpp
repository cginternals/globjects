
#include <glbinding/gl/gl.h>

#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>

#include <globjects/base/File.h>
#include <globjects/Uniform.h>
#include <globjects/Program.h>
#include <globjects/Shader.h>
#include <globjects/Buffer.h>
#include <globjects/Framebuffer.h>
#include <globjects/VertexArray.h>
#include <globjects/Texture.h>

#include <common/AxisAlignedBoundingBox.h>
#include <common/Icosahedron.h>
#include <common/Camera.h>
#include <common/AbstractCoordinateProvider.h>
#include <common/WorldInHandNavigation.h>
#include <common/ScreenAlignedQuad.h>

#include <common/ContextFormat.h>
#include <common/Context.h>
#include <common/Window.h>
#include <common/WindowEventHandler.h>
#include <common/events.h>


using namespace gl;
using namespace glm;
using namespace globjects;

class EventHandler : public WindowEventHandler, AbstractCoordinateProvider
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
        WindowEventHandler::initialize(window);

        glClearColor(1.f, 1.f, 1.f, 0.f);

        m_camera.setZNear(1.f);
        m_camera.setZFar(16.f);

        m_icosahedron.reset(new Icosahedron(2));

        m_sphere.reset(new Program());

        m_vertexSource.reset(new File("data/gbuffers/sphere.vert"));
        m_fragmentSource.reset(new File("data/gbuffers/sphere.frag"));

        m_vertexShader.reset(new Shader(GL_VERTEX_SHADER, m_vertexSource.get()));
        m_fragmentShader.reset(new Shader(GL_FRAGMENT_SHADER, m_fragmentSource.get()));

        m_sphere->attach(
            m_vertexShader.get(),
            m_fragmentShader.get());

        m_colorTexture.reset(Texture::createDefault(GL_TEXTURE_2D));
        m_depthTexture.reset(Texture::createDefault(GL_TEXTURE_2D));
        m_normalTexture.reset(Texture::createDefault(GL_TEXTURE_2D));
        m_geometryTexture.reset(Texture::createDefault(GL_TEXTURE_2D));

        m_sphereFBO.reset(new Framebuffer);
        m_sphereFBO->attachTexture(GL_COLOR_ATTACHMENT0, m_colorTexture.get());
        m_sphereFBO->attachTexture(GL_COLOR_ATTACHMENT1, m_normalTexture.get());
        m_sphereFBO->attachTexture(GL_COLOR_ATTACHMENT2, m_geometryTexture.get());
        m_sphereFBO->attachTexture(GL_DEPTH_ATTACHMENT,  m_depthTexture.get());
        m_sphereFBO->setDrawBuffers({ GL_COLOR_ATTACHMENT0, GL_COLOR_ATTACHMENT1, GL_COLOR_ATTACHMENT2 });

        m_postprocessingSource.reset(new File("data/gbuffers/postprocessing.frag"));
        m_postprocessingShader.reset(new Shader(GL_FRAGMENT_SHADER, m_postprocessingSource.get()));

        m_postprocessingColorSourceUniform.reset(new Uniform<int>("colorSource", 0));
        m_postprocessingNormalSourceUniform.reset(new Uniform<int>("normalSource", 1));
        m_postprocessingWorldCoordSourceUniform.reset(new Uniform<int>("worldCoordSource", 2));
        m_postprocessingDepthSourceUniform.reset(new Uniform<int>("depthSource", 3));

        m_postprocessing.reset(new ScreenAlignedQuad(m_postprocessingShader.get()));
        m_postprocessing->program()->attach(
            m_postprocessingColorSourceUniform.get(),
            m_postprocessingNormalSourceUniform.get(),
            m_postprocessingWorldCoordSourceUniform.get(),
            m_postprocessingDepthSourceUniform.get()
        );

        m_postprocessedTexture.reset(Texture::createDefault(GL_TEXTURE_2D));

        m_postprocessingFBO.reset(new Framebuffer);
        m_postprocessingFBO->attachTexture(GL_COLOR_ATTACHMENT0, m_postprocessedTexture.get());
        m_postprocessingFBO->setDrawBuffer(GL_COLOR_ATTACHMENT0);

        m_gBufferChoiceSource.reset(new File("data/gbuffers/gbufferchoice.frag"));
        m_gBufferChoiceShader.reset(new Shader(GL_FRAGMENT_SHADER, m_gBufferChoiceSource.get()));

        m_gBufferChoice.reset(new ScreenAlignedQuad(m_gBufferChoiceShader.get()));

        m_postprocessedSourceUniform.reset(new Uniform<int>("postprocessedSource", 0));
        m_gBufferChoiceColorSourceUniform.reset(new Uniform<int>("colorSource", 1));
        m_gBufferChoiceNormalSourceUniform.reset(new Uniform<int>("normalSource", 2));
        m_gBufferChoiceWorldCoordSourceUniform.reset(new Uniform<int>("worldCoordSource", 3));
        m_gBufferChoiceDepthSourceUniform.reset(new Uniform<int>("depthSource", 4));

        m_nearPlaneUniform.reset(new Uniform<float>("nearZ", m_camera.zNear()));
        m_farPlaneUniform.reset(new Uniform<float>("farZ", m_camera.zFar()));

        m_gBufferChoice->program()->attach(
            m_postprocessedSourceUniform.get(),
            m_gBufferChoiceColorSourceUniform.get(),
            m_gBufferChoiceNormalSourceUniform.get(),
            m_gBufferChoiceWorldCoordSourceUniform.get(),
            m_gBufferChoiceDepthSourceUniform.get(),
            m_nearPlaneUniform.get(),
            m_farPlaneUniform.get()
        );

        m_screenSizeUniform.reset(new Uniform<glm::vec2>("screenSize", glm::vec2()));

        m_postprocessing->program()->attach(m_screenSizeUniform.get());

        m_viewProjectionUniform.reset(new Uniform<glm::mat4>("transform", m_camera.viewProjection()));
        m_viewUniform.reset(new Uniform<glm::mat4>("modelView", m_camera.view()));
        m_normalMatrixUniform.reset(new Uniform<glm::mat3>("normalMatrix", m_camera.normal()));

        m_sphere->attach(
            m_viewProjectionUniform.get(),
            m_viewUniform.get(),
            m_normalMatrixUniform.get()
        );

        m_choiceUniform.reset(new Uniform<int>("choice", 0));

        m_gBufferChoice->program()->attach(m_choiceUniform.get());

        window.addTimer(0, 0, false);

        cameraChanged();
    }

    virtual void framebufferResizeEvent(ResizeEvent & event) override
    {
        glViewport(0, 0, event.width(), event.height());
        m_camera.setViewport(event.width(), event.height());

        m_screenSizeUniform->set(vec2(event.size()));

        cameraChanged();

        m_colorTexture->image2D(        0, GL_RGBA8,           event.width(), event.height(), 0, GL_RGBA, GL_UNSIGNED_BYTE, nullptr);
        m_normalTexture->image2D(       0, GL_RGBA16F,         event.width(), event.height(), 0, GL_RGBA, GL_FLOAT, nullptr);
        m_geometryTexture->image2D(     0, GL_RGBA16F,         event.width(), event.height(), 0, GL_RGBA, GL_FLOAT, nullptr);
        m_depthTexture->image2D(        0, GL_DEPTH_COMPONENT, event.width(), event.height(), 0, GL_DEPTH_COMPONENT, GL_FLOAT, nullptr);
        m_postprocessedTexture->image2D(0, GL_RGBA8,           event.width(), event.height(), 0, GL_RGBA, GL_UNSIGNED_BYTE, nullptr);
    }

    void cameraChanged()
    {
        m_viewProjectionUniform->set(m_camera.viewProjection());
        m_viewUniform->set(m_camera.view());
        m_normalMatrixUniform->set(m_camera.normal());
    }

    virtual void paintEvent(PaintEvent & event) override
    {
        WindowEventHandler::paintEvent(event);

        // Sphere Pass

        m_sphereFBO->bind();

        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        m_sphere->use();
        m_icosahedron->draw();
        m_sphere->release();

        m_sphereFBO->unbind();

        // Postprocessing Pass

        m_postprocessingFBO->bind();

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

        m_postprocessingFBO->unbind();

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

    virtual void keyPressEvent(KeyEvent & event) override
    {
        WindowEventHandler::keyPressEvent(event);

        switch (event.key())
        {
        case GLFW_KEY_1:
        case GLFW_KEY_2:
        case GLFW_KEY_3:
        case GLFW_KEY_4:
        case GLFW_KEY_5:
            m_choiceUniform->set(static_cast<int>(event.key() - 49));
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
        m_sphereFBO->bind();
        float depth = AbstractCoordinateProvider::depthAt(m_camera, GL_DEPTH_COMPONENT, windowCoordinates);
        m_sphereFBO->unbind();

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
    std::unique_ptr<Icosahedron> m_icosahedron;
    std::unique_ptr<Program> m_sphere;
    std::unique_ptr<Texture> m_colorTexture;
    std::unique_ptr<Texture> m_normalTexture;
    std::unique_ptr<Texture> m_geometryTexture;
    std::unique_ptr<Texture> m_depthTexture;
    std::unique_ptr<Framebuffer> m_sphereFBO;

    std::unique_ptr<AbstractStringSource> m_vertexSource;
    std::unique_ptr<AbstractStringSource> m_fragmentSource;

    std::unique_ptr<Shader> m_vertexShader;
    std::unique_ptr<Shader> m_fragmentShader;

    std::unique_ptr<AbstractStringSource> m_postprocessingSource;
    std::unique_ptr<Shader> m_postprocessingShader;

    std::unique_ptr<AbstractStringSource> m_gBufferChoiceSource;
    std::unique_ptr<Shader> m_gBufferChoiceShader;

    std::unique_ptr<ScreenAlignedQuad> m_postprocessing;
    std::unique_ptr<Texture> m_postprocessedTexture;
    std::unique_ptr<Framebuffer> m_postprocessingFBO;

    std::unique_ptr<ScreenAlignedQuad> m_gBufferChoice;

    std::unique_ptr<Uniform<int>> m_choiceUniform;

    std::unique_ptr<Uniform<int>> m_postprocessingColorSourceUniform;
    std::unique_ptr<Uniform<int>> m_postprocessingNormalSourceUniform;
    std::unique_ptr<Uniform<int>> m_postprocessingWorldCoordSourceUniform;
    std::unique_ptr<Uniform<int>> m_postprocessingDepthSourceUniform;

    std::unique_ptr<Uniform<int>> m_postprocessedSourceUniform;
    std::unique_ptr<Uniform<int>> m_gBufferChoiceColorSourceUniform;
    std::unique_ptr<Uniform<int>> m_gBufferChoiceNormalSourceUniform;
    std::unique_ptr<Uniform<int>> m_gBufferChoiceWorldCoordSourceUniform;
    std::unique_ptr<Uniform<int>> m_gBufferChoiceDepthSourceUniform;

    std::unique_ptr<Uniform<float>> m_nearPlaneUniform;
    std::unique_ptr<Uniform<float>> m_farPlaneUniform;

    std::unique_ptr<Uniform<glm::vec2>> m_screenSizeUniform;

    std::unique_ptr<Uniform<glm::mat4>> m_viewProjectionUniform;
    std::unique_ptr<Uniform<glm::mat4>> m_viewUniform;
    std::unique_ptr<Uniform<glm::mat3>> m_normalMatrixUniform;

    Camera m_camera;
    WorldInHandNavigation m_nav;
    ivec2 m_lastMousePos;

    AxisAlignedBoundingBox m_aabb;
};


int main(int /*argc*/, char * /*argv*/[])
{
    info() << "Usage:";
    info() << "\t" << "ESC" << "\t\t"        << "Close example";
    info() << "\t" << "ALT + Enter"          << "\t" << "Toggle fullscreen";
    info() << "\t" << "F11" << "\t\t"        << "Toggle fullscreen";
    info() << "\t" << "F10" << "\t\t"        << "Toggle vertical sync";
    info() << "\t" << "F5" << "\t\t"         << "Reload shaders";
    info() << "\t" << "Space" << "\t\t"      << "Reset camera";
    info() << "\t" << "Left Mouse" << "\t"   << "Pan scene";
    info() << "\t" << "Right Mouse" << "\t"  << "Rotate scene";
    info() << "\t" << "Mouse Wheel" << "\t"  << "Zoom scene";

    info() << "\nSwitch between G-Buffers";
    info() << "\t" << "1" << "\t" << "Postprocessed";
    info() << "\t" << "2" << "\t" << "Color";
    info() << "\t" << "3" << "\t" << "Normal";
    info() << "\t" << "4" << "\t" << "Geometry";
    info() << "\t" << "5" << "\t" << "Depth";

    ContextFormat format;
    format.setVersion(3, 2);
    format.setProfile(ContextFormat::Profile::Core);
    format.setForwardCompatible(true);

    Window::init();

    Window window;
    window.setEventHandler(new EventHandler());

    if (!window.create(format, "GBuffers Example"))
        return 1;

    window.show();

    return MainLoop::run();
}
