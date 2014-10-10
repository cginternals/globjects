#include "WindowQt.h"

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

WindowQt::WindowQt()
: WindowQt(defaultFormat())
{
}

WindowQt::WindowQt(const QSurfaceFormat & format)
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
}

WindowQt::~WindowQt()
{
}

QOpenGLContext * WindowQt::context()
{
    return m_context.data();
}

void WindowQt::makeCurrent()
{
    m_context->makeCurrent(this);
}

void WindowQt::doneCurrent()
{
    m_context->doneCurrent();
}

void WindowQt::resizeEvent(QResizeEvent * event)
{
    resize(event);
    paint();
}

void WindowQt::exposeEvent(QExposeEvent * )
{
    paint();
}

void WindowQt::initialize()
{
    makeCurrent();

    initializeGL();

    doneCurrent();

    m_initialized = true;
}

void WindowQt::resize(QResizeEvent * event)
{
    if (!m_initialized)
        initialize();

    makeCurrent();

    QResizeEvent deviceSpecificResizeEvent(event->size() * devicePixelRatio(), event->oldSize() * devicePixelRatio());

    resizeGL(&deviceSpecificResizeEvent);

    doneCurrent();
}

void WindowQt::paint()
{
    if (!m_initialized)
        initialize();

    if (!isExposed())
        return;

    m_updatePending = false;

    makeCurrent();

    paintGL();

    m_context->swapBuffers(this);

    doneCurrent();
}

void WindowQt::updateGL()
{
    if (!m_updatePending)
    {
        m_updatePending = true;
        QCoreApplication::postEvent(this, new QEvent(QEvent::UpdateRequest));
    }
}

bool WindowQt::event(QEvent * event)
{
    switch (event->type())
    {
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

void WindowQt::initializeGL()
{
}

void WindowQt::resizeGL(QResizeEvent *)
{
}

void WindowQt::paintGL()
{
}

void WindowQt::enterEvent(QEvent *)
{
}

void WindowQt::leaveEvent(QEvent *)
{
}
