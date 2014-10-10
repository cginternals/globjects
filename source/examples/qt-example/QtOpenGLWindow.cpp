#include "QtOpenGLWindow.h"

#ifdef __GNUC__
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wfloat-equal"
#pragma GCC diagnostic ignored "-Wconversion"
#pragma GCC diagnostic ignored "-Wswitch"
#endif
#include <QApplication>
#include <QDebug>
#include <QResizeEvent>
#include <QSurfaceFormat>
#include <QOpenGLContext>
#ifdef __GNUC__
#pragma GCC diagnostic pop
#endif

QSurfaceFormat defaultFormat()
{
    QSurfaceFormat format;
    format.setProfile(QSurfaceFormat::CoreProfile);
#ifndef NDEBUG
    format.setOption(QSurfaceFormat::DebugContext);
#endif
    return format;
}

QtOpenGLWindow::QtOpenGLWindow()
: QtOpenGLWindow(defaultFormat())
{
}

QtOpenGLWindow::QtOpenGLWindow(const QSurfaceFormat& format)
: m_context(new QOpenGLContext)
, m_updatePending(false)
, m_initialized(false)
{
    QSurfaceFormat f(format);
    f.setRenderableType(QSurfaceFormat::OpenGL);

    setSurfaceType(OpenGLSurface);
    create();

    m_context->setFormat(format);
    if (!m_context->create())
    {
        qDebug() << "Could not create OpenGL context.";
        QApplication::quit();
    }
    else
    {
        qDebug().nospace() << "Created OpenGL context " << m_context->format().version().first << "." << m_context->format().version().second;
    }
}

QtOpenGLWindow::~QtOpenGLWindow()
{
}

QOpenGLContext* QtOpenGLWindow::context()
{
    return m_context.data();
}

void QtOpenGLWindow::makeCurrent()
{
    m_context->makeCurrent(this);
}

void QtOpenGLWindow::doneCurrent()
{
    m_context->doneCurrent();
}

void QtOpenGLWindow::resizeEvent(QResizeEvent * event)
{
    resize(event);
    paint();
}

void QtOpenGLWindow::exposeEvent(QExposeEvent * )
{
    paint();
}

void QtOpenGLWindow::initialize()
{
    makeCurrent();

    initializeGL();

    doneCurrent();

    m_initialized = true;
}

void QtOpenGLWindow::resize(QResizeEvent * event)
{
    if (!m_initialized)
    {
        initialize();
    }

    makeCurrent();

    QResizeEvent deviceSpecificResizeEvent(event->size() * devicePixelRatio(), event->oldSize() * devicePixelRatio());

    resizeGL(&deviceSpecificResizeEvent);

    doneCurrent();
}

void QtOpenGLWindow::paint()
{
    if (!m_initialized)
    {
        initialize();
    }

    if (!isExposed())
        return;

    m_updatePending = false;

    makeCurrent();

    paintGL();

    m_context->swapBuffers(this);

    doneCurrent();
}

void QtOpenGLWindow::updateGL()
{
    if (!m_updatePending)
    {
        m_updatePending = true;
        QCoreApplication::postEvent(this, new QEvent(QEvent::UpdateRequest));
    }
}

bool QtOpenGLWindow::event(QEvent * event)
{
    switch (event->type()) {
        case QEvent::UpdateRequest:
            paint();
            return true;
        case QEvent::Enter:
            enterEvent(event);
            return true;
        case QEvent::Leave:
            leaveEvent(event);
            return true;
        default:
            return QWindow::event(event);
    }
}

void QtOpenGLWindow::initializeGL()
{
}

void QtOpenGLWindow::resizeGL(QResizeEvent * )
{
}

void QtOpenGLWindow::paintGL()
{
}

void QtOpenGLWindow::enterEvent(QEvent *)
{
}

void QtOpenGLWindow::leaveEvent(QEvent *)
{
}
