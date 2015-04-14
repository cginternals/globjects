
#ifdef __GNUC__
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wfloat-equal"
#pragma GCC diagnostic ignored "-Wconversion"
#pragma GCC diagnostic ignored "-Wswitch"
#endif

#include <iostream>

#include <QApplication>
#include <QMainWindow>
#include <QTimer>
#include <QResizeEvent>
#include <QWheelEvent>
#include <QSurfaceFormat>

#ifdef __GNUC__
#pragma GCC diagnostic pop
#endif

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/transform.hpp>

#include <glbinding/gl/gl.h>
#include <glbinding/ContextInfo.h>

#include <globjects/globjects.h>
#include <globjects/Uniform.h>
#include <globjects/Program.h>
#include <globjects/Shader.h>
#include <globjects/Buffer.h>
#include <globjects/logging.h>
#include <globjects/VertexArray.h>
#include <globjects/DebugMessage.h>

#include <globjects/base/File.h>

#include <common/AxisAlignedBoundingBox.h>
#include <common/Icosahedron.h>
#include <common/Camera.h>
#include <common/AbstractCoordinateProvider.h>
#include <common/WorldInHandNavigation.h>

#include "WindowQt.h"


using namespace gl;
using namespace globjects;

class Window : public WindowQt, AbstractCoordinateProvider
{
public:
    Window(QSurfaceFormat & format)
    : WindowQt(format)
    , m_camera(glm::vec3(0.f, 1.f, 4.f))
    {
        m_aabb.extend(glm::vec3(-8.f, -1.f, -8.f));
        m_aabb.extend(glm::vec3( 8.f,  1.f,  8.f));

        m_nav.setCamera(&m_camera);
        m_nav.setCoordinateProvider(this);
        m_nav.setBoundaryHint(m_aabb);

        m_timer = new QTimer(this);
        m_timer->setInterval(0);

        connect(m_timer, &QTimer::timeout, this, &WindowQt::updateGL);
    }

    virtual ~Window()
    {
    }

    virtual void initializeGL() override
    {
        init();
        DebugMessage::enable();

        std::cout << std::endl
            << "OpenGL Version:  " << glbinding::ContextInfo::version() << std::endl
            << "OpenGL Vendor:   " << glbinding::ContextInfo::vendor() << std::endl
            << "OpenGL Renderer: " << glbinding::ContextInfo::renderer() << std::endl << std::endl;

#ifdef __APPLE__
        Shader::clearGlobalReplacements();
        Shader::globalReplace("#version 140", "#version 150");

        debug() << "Using global OS X shader replacement '#version 140' -> '#version 150'" << std::endl;
#endif

        glClearColor(1.f, 1.f, 1.f, 0.f);

        m_vertexSource.reset(new File("data/gbuffers/sphere.vert"));
        m_vertexShader.reset(new Shader(GL_VERTEX_SHADER, m_vertexSource.get()));

        m_fragmentSource.reset(new File("data/gbuffers/sphere.frag"));
        m_fragmentShader.reset(new Shader(GL_FRAGMENT_SHADER, m_fragmentSource.get()));

        m_sphere.reset(new Program());
        m_sphere->attach(m_vertexShader.get(), m_fragmentShader.get());

        m_icosahedron.reset(new Icosahedron(2));

        m_camera.setZNear(1.f);
        m_camera.setZFar(16.f);

        m_transformUniform.reset(new Uniform<glm::mat4>("transform", m_camera.viewProjection()));

        m_sphere->attach(m_transformUniform.get());

        m_timer->start();
    }

    virtual void resizeGL(QResizeEvent * event) override
    {
        glViewport(0, 0, event->size().width(), event->size().height());

        m_camera.setViewport(event->size().width(), event->size().height());
    }

    virtual void paintGL() override
    {
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        m_transformUniform->set(m_camera.viewProjection());

        m_sphere->use();
        m_icosahedron->draw();
        m_sphere->release();
    }


    virtual void keyPressEvent(QKeyEvent * event) override
    {
        makeCurrent();

        switch (event->key())
        {
        case Qt::Key_F5:
            File::reloadAll();
            break;

        case Qt::Key_Space:
            m_camera.setCenter(glm::vec3());
            m_camera.setEye(glm::vec3( 0.f, 1.f, 4.f));
            m_camera.setUp (glm::vec3( 0.f, 1.f, 0.f));
            break;
        }
        doneCurrent();
    }

    virtual void mousePressEvent(QMouseEvent * event) override
    {
        glm::ivec2 pos(event->pos().x(), event->pos().y());

        makeCurrent();

        switch (event->button())
        {
        case Qt::LeftButton:
            m_nav.panBegin(pos);
            break;

        case Qt::RightButton:
            m_nav.rotateBegin(pos);
            break;

        default:
            break;
        }

        doneCurrent();
    }

    virtual void mouseMoveEvent(QMouseEvent * event) override
    {
        glm::ivec2 pos(event->pos().x(), event->pos().y());

        makeCurrent();

        switch (m_nav.mode())
        {
        case WorldInHandNavigation::PanInteraction:
            m_nav.panProcess(pos);
            break;

        case WorldInHandNavigation::RotateInteraction:
            m_nav.rotateProcess(pos);
            break;

        case WorldInHandNavigation::NoInteraction:
            break;
        }

        doneCurrent();
    }

    virtual void mouseReleaseEvent(QMouseEvent * event) override
    {
        makeCurrent();

        switch (event->button())
        {
        case Qt::LeftButton:
            m_nav.panEnd();
            break;

        case Qt::RightButton:
            m_nav.rotateEnd();
            break;

        default:
            break;
        }

        doneCurrent();
    }

    virtual void wheelEvent(QWheelEvent * event) override
    {
        if (WorldInHandNavigation::NoInteraction != m_nav.mode())
            return;

        makeCurrent();

        glm::vec2 pos(event->pos().x(), event->pos().y());

        m_nav.scaleAtMouse(glm::ivec2(pos), static_cast<float>(-event->delta()) * 0.001f);

        doneCurrent();
    }

    virtual float depthAt(const glm::ivec2 & windowCoordinates) const override
    {
        return AbstractCoordinateProvider::depthAt(m_camera, GL_DEPTH_COMPONENT, windowCoordinates);
    }

    virtual glm::vec3 objAt(const glm::ivec2 & windowCoordinates) const override
    {
        return unproject(m_camera, static_cast<GLenum>(GL_DEPTH_COMPONENT), windowCoordinates);
    }

    virtual glm::vec3 objAt(const glm::ivec2 & windowCoordinates, const float depth) const override
    {
        return unproject(m_camera, depth, windowCoordinates);
    }

    virtual glm::vec3 objAt(const glm::ivec2 & windowCoordinates, const float depth, const glm::mat4 & viewProjectionInverted) const override
    {
        return unproject(m_camera, viewProjectionInverted, depth, windowCoordinates);
    }

protected:
    std::unique_ptr<Program> m_sphere;
    std::unique_ptr<AbstractStringSource> m_vertexSource;
    std::unique_ptr<AbstractStringSource> m_fragmentSource;
    std::unique_ptr<Shader> m_vertexShader;
    std::unique_ptr<Shader> m_fragmentShader;

    std::unique_ptr<Uniform<glm::mat4>> m_transformUniform;

    std::unique_ptr<Icosahedron> m_icosahedron;

    Camera m_camera;
    WorldInHandNavigation m_nav;

    QTimer * m_timer;

    AxisAlignedBoundingBox m_aabb;
};

int main(int argc, char * argv[])
{
    QApplication app(argc, argv);

    QSurfaceFormat format;
#ifdef __APPLE__
    format.setVersion(3, 2);
    format.setProfile(QSurfaceFormat::CoreProfile);
#else
    format.setVersion(3, 1);
#endif
    format.setDepthBufferSize(16);

    Window * glwindow = new Window(format);

    QMainWindow window;
    window.setGeometry(0, 0, 640, 480);
    window.setCentralWidget(QWidget::createWindowContainer(glwindow));

    window.show();

    return app.exec();
}
