#pragma once


#include <QWindow>
#include <QScopedPointer>

#include <glbinding/ProcAddress.h>


class QSurfaceFormat;
class QOpenGLContext;

class WindowQt : public QWindow
{
public:
    WindowQt(QApplication & app, const QSurfaceFormat & format);
    virtual ~WindowQt();

    virtual void resizeEvent(QResizeEvent * event) override;
    virtual void exposeEvent(QExposeEvent * event) override;
    bool event(QEvent * event) override;

    virtual void enterEvent(QEvent * event);
    virtual void leaveEvent(QEvent * event);

    void makeCurrent();
    void doneCurrent();

    QOpenGLContext * context();

    void updateGL();

protected:
    QScopedPointer<QOpenGLContext> m_context;

    bool m_updatePending;
    bool m_initialized;

    void initialize();
    void resize(QResizeEvent * event);
    void paint();

    virtual bool initializeGL();
    virtual void deinitializeGL();
    virtual void resizeGL(QResizeEvent * event);
    virtual void paintGL();

protected:
    static WindowQt * s_getProcAddressHelper;
    static glbinding::ProcAddress getProcAddress(const char * name);
};
