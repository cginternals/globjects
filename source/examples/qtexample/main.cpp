
#include "QtOpenGLWindow.h"

#ifdef __GNUC__
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wfloat-equal"
#pragma GCC diagnostic ignored "-Wconversion"
#pragma GCC diagnostic ignored "-Wswitch"
#endif
#include <QApplication>
#include <QMainWindow>
#include <QTimer>
#include <QResizeEvent>
#include <QWheelEvent>
#ifdef __GNUC__
#pragma GCC diagnostic pop
#endif

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/transform.hpp>

#include <glbinding/gl/gl.h>

#include <glow/glow.h>
#include <glow/Uniform.h>
#include <glow/Program.h>
#include <glow/Shader.h>
#include <glow/Buffer.h>
#include <glow/logging.h>
#include <glow/VertexArrayObject.h>
#include <glow/DebugMessage.h>

#include <glowutils/AxisAlignedBoundingBox.h>
#include <glowutils/Icosahedron.h>
#include <glowutils/Camera.h>
#include <glowutils/AdaptiveGrid.h>
#include <glowutils/AbstractCoordinateProvider.h>
#include <glowutils/WorldInHandNavigation.h>
#include <glowutils/FlightNavigation.h>
#include <glowbase/File.h>
#include <glowutils/glowutils.h>
#include <glowutils/StringTemplate.h>

using namespace glm;

class Window : public QtOpenGLWindow, glowutils::AbstractCoordinateProvider
{
public:
    Window()
    : QtOpenGLWindow()
    , m_camera(vec3(0.f, 1.f, 4.0f))
    {
        m_aabb.extend(vec3(-8.f, -1.f, -8.f));
        m_aabb.extend(vec3(8.f, 1.f, 8.f));

        m_nav.setCamera(&m_camera);
        m_nav.setCoordinateProvider(this);
        m_nav.setBoundaryHint(m_aabb);

        m_timer = new QTimer(this);
        m_timer->setInterval(0);

        connect(m_timer, &QTimer::timeout, this, &QtOpenGLWindow::updateGL);
    }

    virtual ~Window()
    {
    }

    virtual void initializeGL() override
    {
        glow::init();

        glow::DebugMessage::enable();

        gl::glClearColor(1.0f, 1.0f, 1.0f, 0.f);

        m_sphere = new glow::Program();
        glowutils::StringTemplate* vertexShaderSource = new glowutils::StringTemplate(new glow::File("data/adaptive-grid/sphere.vert"));
        glowutils::StringTemplate* fragmentShaderSource = new glowutils::StringTemplate(new glow::File("data/adaptive-grid/sphere.frag"));

#ifdef MAC_OS
        vertexShaderSource->replace("#version 140", "#version 150");
        fragmentShaderSource->replace("#version 140", "#version 150");
#endif

        m_sphere->attach(
            new glow::Shader(gl::GL_VERTEX_SHADER, vertexShaderSource)
        ,   new glow::Shader(gl::GL_FRAGMENT_SHADER, fragmentShaderSource));


        m_icosahedron = new glowutils::Icosahedron(2);
        m_agrid = new glowutils::AdaptiveGrid(16U);

        m_camera.setZNear(0.1f);
        m_camera.setZFar(1024.f);

        m_agrid->setCamera(&m_camera);

        m_timer->start();
    }

    virtual void resizeGL(QResizeEvent * event) override
    {
        gl::glViewport(0, 0, event->size().width(), event->size().height());

        m_camera.setViewport(event->size().width(), event->size().height());
    }

    virtual void paintGL() override
    {
        gl::glClear(gl::GL_COLOR_BUFFER_BIT | gl::GL_DEPTH_BUFFER_BIT);

        m_agrid->update();
        m_sphere->setUniform("transform", m_camera.viewProjection());

        m_sphere->use();
        m_icosahedron->draw();
        m_sphere->release();

        m_agrid->draw();
    }


    virtual void keyPressEvent(QKeyEvent * event) override
    {
        makeCurrent();

        switch (event->key())
        {
            case Qt::Key_F5:
                glow::File::reloadAll();
                break;
            case Qt::Key_Space:
                m_camera.setCenter(vec3());
                m_camera.setEye(vec3(0.f, 1.f, 4.0f));
                m_camera.setUp(vec3(0,1,0));
                break;
        }

        doneCurrent();
    }

    virtual void mousePressEvent(QMouseEvent * event) override
    {
        ivec2 pos(event->pos().x(), event->pos().y());

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
        ivec2 pos(event->pos().x(), event->pos().y());

        makeCurrent();

        switch (m_nav.mode())
        {
            case glowutils::WorldInHandNavigation::PanInteraction:
                m_nav.panProcess(pos);
                break;

            case glowutils::WorldInHandNavigation::RotateInteraction:
                m_nav.rotateProcess(pos);
                break;
            case glowutils::WorldInHandNavigation::NoInteraction:
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
        if (glowutils::WorldInHandNavigation::NoInteraction != m_nav.mode())
            return;

        makeCurrent();

        ivec2 pos(event->pos().x(), event->pos().y());

        m_nav.scaleAtMouse(pos, static_cast<float>(-event->delta()) * 0.001f);

        doneCurrent();
    }

    virtual float depthAt(const ivec2 & windowCoordinates) const override
    {
        return AbstractCoordinateProvider::depthAt(m_camera, gl::GL_DEPTH_COMPONENT, windowCoordinates);
    }

    virtual vec3 objAt(const ivec2 & windowCoordinates) const override
    {
        return unproject(m_camera, static_cast<gl::GLenum>(gl::GL_DEPTH_COMPONENT), windowCoordinates);
    }

    virtual vec3 objAt(const ivec2 & windowCoordinates, const float depth) const override
    {
        return unproject(m_camera, depth, windowCoordinates);
    }

    virtual glm::vec3 objAt(const ivec2 & windowCoordinates, const float depth, const mat4 & viewProjectionInverted) const override
    {
        return unproject(m_camera, viewProjectionInverted, depth, windowCoordinates);
    }

protected:
    glow::ref_ptr<glow::Program> m_sphere;

    glow::ref_ptr<glowutils::Icosahedron> m_icosahedron;
    glow::ref_ptr<glowutils::AdaptiveGrid> m_agrid;

    glowutils::Camera m_camera;
    glowutils::WorldInHandNavigation m_nav;

    QTimer * m_timer;

    glowutils::AxisAlignedBoundingBox m_aabb;
};

int main(int argc, char* argv[])
{
    QApplication app(argc, argv);

    Window * openGLwindow = new Window();

    QMainWindow window;
    window.setGeometry(100, 100, 800, 600);
    window.setCentralWidget(QWidget::createWindowContainer(openGLwindow));

    window.show();

    return app.exec();
}
