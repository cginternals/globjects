#pragma once

#ifdef __GNUC__
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wfloat-equal"
#pragma GCC diagnostic ignored "-Wconversion"
#pragma GCC diagnostic ignored "-Wswitch"
#endif
#include <QWindow>
#include <QScopedPointer>
#ifdef __GNUC__
#pragma GCC diagnostic pop
#endif

class QSurfaceFormat;
class QOpenGLContext;

class QtOpenGLWindow : public QWindow
{
public:
    QtOpenGLWindow();
    QtOpenGLWindow(const QSurfaceFormat& format);
    virtual ~QtOpenGLWindow();

    virtual void resizeEvent(QResizeEvent * event) override;
    virtual void exposeEvent(QExposeEvent * event) override;
    bool event(QEvent * event);

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

    virtual void initializeGL();
    virtual void resizeGL(QResizeEvent * event);
    virtual void paintGL();
};
