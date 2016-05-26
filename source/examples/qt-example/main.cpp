
#ifdef __GNUC__
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wfloat-equal"
#pragma GCC diagnostic ignored "-Wconversion"
#pragma GCC diagnostic ignored "-Wswitch"
#endif

#include <iostream>
#include <algorithm>

#include <cpplocate/cpplocate.h>
#include <cpplocate/ModuleInfo.h>

#include <QApplication>
#include <QMainWindow>
#include <QResizeEvent>
#include <QSurfaceFormat>

#ifdef __GNUC__
#pragma GCC diagnostic pop
#endif

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/transform.hpp>

#include <glbinding/gl/gl.h>
#include <glbinding/ContextInfo.h>
#include <glbinding/Version.h>

#include <globjects/globjects.h>
#include <globjects/base/File.h>
#include <globjects/logging.h>

#include <globjects/Buffer.h>
#include <globjects/Program.h>
#include <globjects/Shader.h>
#include <globjects/VertexArray.h>
#include <globjects/VertexAttributeBinding.h>
#include <globjects/base/StaticStringSource.h>

#include "WindowQt.h"


using namespace gl;
using namespace globjects;


namespace
{

// taken from iozeug::FilePath::toPath
std::string normalizePath(const std::string & filepath)
{
    auto copy = filepath;
    std::replace( copy.begin(), copy.end(), '\\', '/');
    auto i = copy.find_last_of('/');
    if (i == copy.size()-1)
    {
        copy = copy.substr(0, copy.size()-1);
    }
    return copy;
}

}


class Window : public WindowQt
{
public:
    Window(QSurfaceFormat & format)
    : WindowQt(format)
    {
    }

    virtual ~Window()
    {
    }

    virtual void initializeGL() override
    {
        cpplocate::ModuleInfo moduleInfo = cpplocate::findModule("globjects");

        // Get data path
        std::string dataPath = moduleInfo.value("dataPath");
        dataPath = normalizePath(dataPath);
        if (dataPath.size() > 0) dataPath = dataPath + "/";
        else                     dataPath = "data/";

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

        glClearColor(0.2f, 0.3f, 0.4f, 1.f);

        // Initialize OpenGL objects
        m_cornerBuffer = new Buffer();
        m_program = new Program();
        m_vao = new VertexArray();

        m_program->attach(
            Shader::fromFile(GL_VERTEX_SHADER,  dataPath + "qt-example/shader.vert"),
            Shader::fromFile(GL_FRAGMENT_SHADER, dataPath +"qt-example/shader.frag"));

        m_cornerBuffer->setData(std::array<glm::vec2, 4>{ {
                glm::vec2(0, 0), glm::vec2(1, 0), glm::vec2(0, 1), glm::vec2(1, 1) } }, GL_STATIC_DRAW);

        m_vao->binding(0)->setAttribute(0);
        m_vao->binding(0)->setBuffer(m_cornerBuffer, 0, sizeof(glm::vec2));
        m_vao->binding(0)->setFormat(2, GL_FLOAT);
        m_vao->enable(0);
    }

    virtual void resizeGL(QResizeEvent * event) override
    {
        glViewport(0, 0, event->size().width(), event->size().height());
    }

    virtual void paintGL() override
    {
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        m_program->use();
        m_vao->drawArrays(GL_TRIANGLE_STRIP, 0, 4);
    }


    virtual void keyPressEvent(QKeyEvent * event) override
    {
        makeCurrent();

        switch (event->key())
        {
        case Qt::Key_F5:
            File::reloadAll();
            break;
        default:
            break;
        }
        doneCurrent();
    }


protected:
    ref_ptr<Buffer> m_cornerBuffer;
    ref_ptr<Program> m_program;
    ref_ptr<VertexArray> m_vao;
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
    window.setGeometry(0, 0, 1024, 768);
    window.setCentralWidget(QWidget::createWindowContainer(glwindow));

    window.show();

    return app.exec();
}
