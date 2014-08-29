
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

#include <globjects/globjects.h>
#include <globjects/Uniform.h>
#include <globjects/Program.h>
#include <globjects/Shader.h>
#include <globjects/Buffer.h>
#include <globjects/logging.h>
#include <globjects/VertexArray.h>
#include <globjects/DebugMessage.h>

#include <globjects-utils/AxisAlignedBoundingBox.h>
#include <globjects-utils/Icosahedron.h>
#include <globjects-utils/Camera.h>
#include <globjects-utils/AdaptiveGrid.h>
#include <globjects-utils/AbstractCoordinateProvider.h>
#include <globjects-utils/WorldInHandNavigation.h>
#include <globjects-utils/FlightNavigation.h>
#include <globjects-base/File.h>
#include <globjects-utils/globjects-utils.h>
#include <globjects-utils/StringTemplate.h>

class Window : public QtOpenGLWindow, gloutils::AbstractCoordinateProvider
{
public:
    Window()
    : QtOpenGLWindow()
    , m_camera(glm::vec3(0.f, 1.f, 4.0f))
    {
        m_aabb.extend(glm::vec3(-8.f, -1.f, -8.f));
        m_aabb.extend(glm::vec3(8.f, 1.f, 8.f));

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
        glo::init();

        glo::DebugMessage::enable();

        gl::glClearColor(1.0f, 1.0f, 1.0f, 0.f);

        m_sphere = new glo::Program();
        gloutils::StringTemplate* vertexShaderSource = new gloutils::StringTemplate(new glo::File("data/adaptive-grid/sphere.vert"));
        gloutils::StringTemplate* fragmentShaderSource = new gloutils::StringTemplate(new glo::File("data/adaptive-grid/sphere.frag"));

#ifdef MAC_OS
        vertexShaderSource->replace("#version 140", "#version 150");
        fragmentShaderSource->replace("#version 140", "#version 150");
#endif

        m_sphere->attach(
            new glo::Shader(gl::GL_VERTEX_SHADER, vertexShaderSource)
        ,   new glo::Shader(gl::GL_FRAGMENT_SHADER, fragmentShaderSource));


        m_icosahedron = new gloutils::Icosahedron(2);
        m_agrid = new gloutils::AdaptiveGrid(16U);

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
                glo::File::reloadAll();
                break;
            case Qt::Key_Space:
                m_camera.setCenter(glm::vec3());
                m_camera.setEye(glm::vec3(0.f, 1.f, 4.0f));
                m_camera.setUp(glm::vec3(0,1,0));
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
            case gloutils::WorldInHandNavigation::PanInteraction:
                m_nav.panProcess(pos);
                break;

            case gloutils::WorldInHandNavigation::RotateInteraction:
                m_nav.rotateProcess(pos);
                break;
            case gloutils::WorldInHandNavigation::NoInteraction:
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
        if (gloutils::WorldInHandNavigation::NoInteraction != m_nav.mode())
            return;

        makeCurrent();

        glm::vec2 pos(event->pos().x(), event->pos().y());

        m_nav.scaleAtMouse(glm::ivec2(pos), static_cast<float>(-event->delta()) * 0.001f);

        doneCurrent();
    }

    virtual float depthAt(const glm::ivec2 & windowCoordinates) const override
    {
        return AbstractCoordinateProvider::depthAt(m_camera, gl::GL_DEPTH_COMPONENT, windowCoordinates);
    }

    virtual glm::vec3 objAt(const glm::ivec2 & windowCoordinates) const override
    {
        return unproject(m_camera, static_cast<gl::GLenum>(gl::GL_DEPTH_COMPONENT), windowCoordinates);
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
    glo::ref_ptr<glo::Program> m_sphere;

    glo::ref_ptr<gloutils::Icosahedron> m_icosahedron;
    glo::ref_ptr<gloutils::AdaptiveGrid> m_agrid;

    gloutils::Camera m_camera;
    gloutils::WorldInHandNavigation m_nav;

    QTimer * m_timer;

    gloutils::AxisAlignedBoundingBox m_aabb;
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
