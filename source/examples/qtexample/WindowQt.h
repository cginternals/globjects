#pragma once


#pragma warning(push)
#pragma warning(disable: 4127)
#include <QWindow>
#include <QScopedPointer>
#pragma warning(pop)


class QSurfaceFormat;
class QOpenGLContext;

class WindowQt : public QWindow
{
public:
    WindowQt();
    WindowQt(const QSurfaceFormat & format);
    virtual ~WindowQt();

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
