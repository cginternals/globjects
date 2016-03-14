
#include <chrono>
#include <algorithm>

#include <cpplocate/cpplocate.h>
#include <cpplocate/ModuleInfo.h>

#include <glm/glm.hpp>
#include <glm/gtx/transform.hpp>

#include <glbinding/gl/gl.h>
#include <glbinding/ContextInfo.h>
#include <glbinding/Version.h>

#include <GLFW/glfw3.h>

#include <globjects/globjects.h>
#include <globjects/base/File.h>
#include <globjects/logging.h>

#include <globjects/Uniform.h>
#include <globjects/Program.h>
#include <globjects/Shader.h>
#include <globjects/Buffer.h>
#include <globjects/VertexArray.h>
#include <globjects/VertexAttributeBinding.h>
#include <globjects/TransformFeedback.h>


using namespace gl;
using namespace glm;
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


namespace {
    bool g_toggleFS = false;
    bool g_isFS = false;

    VertexArray * g_vao = nullptr;
    Program * g_transformFeedbackProgram = nullptr;
    TransformFeedback * g_transformFeedback = nullptr;
    Program * g_shaderProgram = nullptr;
    Buffer * g_vertexBuffer1 = nullptr;
    Buffer * g_vertexBuffer2 = nullptr;
    Buffer * g_colorBuffer = nullptr;
    
    std::chrono::high_resolution_clock::time_point g_startTime;
}


void key_callback(GLFWwindow * window, int key, int /*scancode*/, int action, int /*modes*/)
{
    if (key == GLFW_KEY_ESCAPE && action == GLFW_RELEASE)
        glfwSetWindowShouldClose(window, true);

    if (key == GLFW_KEY_F5 && action == GLFW_RELEASE)
        File::reloadAll();

    if (key == GLFW_KEY_F11 && action == GLFW_RELEASE)
        g_toggleFS = true;
}

GLFWwindow * createWindow(bool fs = false)
{
    // Set GLFW window hints
    glfwSetErrorCallback( [] (int /*error*/, const char * description) { puts(description); } );
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 0);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, true);

    // Create a context and, if valid, make it current
    GLFWwindow * window = glfwCreateWindow(1024, 768, "", fs ? glfwGetPrimaryMonitor() : NULL, NULL);
    if (window == nullptr)
    {
        critical() << "Context creation failed. Terminate execution.";

        glfwTerminate();
        exit(1);
    }
    glfwMakeContextCurrent(window);

    // Create callback that when user presses ESC, the context should be destroyed and window closed
    glfwSetKeyCallback(window, key_callback);

    // Initialize globjects (internally initializes glbinding, and registers the current context)
    globjects::init();

    // Do only on startup
    if (!g_toggleFS)
    {
       // Dump information about context and graphics card
       info() << std::endl
           << "OpenGL Version:  " << glbinding::ContextInfo::version() << std::endl
           << "OpenGL Vendor:   " << glbinding::ContextInfo::vendor() << std::endl
           << "OpenGL Renderer: " << glbinding::ContextInfo::renderer() << std::endl;
    }

    if (!hasExtension(GLextension::GL_ARB_transform_feedback3))
    {
        critical() << "Transform feedback not supported.";

        glfwTerminate();
        exit(1);
    }

    glClearColor(0.2f, 0.3f, 0.4f, 1.f);

    g_isFS = fs;
    return window;
}

void destroyWindow(GLFWwindow * window)
{
    globjects::detachAllObjects();
    glfwDestroyWindow(window);
}

void initialize()
{
    cpplocate::ModuleInfo moduleInfo = cpplocate::findModule("globjects");

    // Get data path
    std::string dataPath = moduleInfo.value("dataPath");
    dataPath = normalizePath(dataPath);
    if (dataPath.size() > 0) dataPath = dataPath + "/";
    else                     dataPath = "data/";

    // Initialize OpenGL objects
    g_shaderProgram = new Program();
    g_shaderProgram->ref();
    g_shaderProgram->attach(
        Shader::fromFile(GL_VERTEX_SHADER,   dataPath + "transformfeedback/simple.vert")
      , Shader::fromFile(GL_FRAGMENT_SHADER, dataPath + "transformfeedback/simple.frag"));

    g_transformFeedbackProgram = new Program();
    g_transformFeedbackProgram->ref();
    g_transformFeedbackProgram->attach(
        Shader::fromFile(GL_VERTEX_SHADER, dataPath + "transformfeedback/transformfeedback.vert"));

    g_transformFeedbackProgram->setUniform("deltaT", 0.0f);


    const int width = 1024;
    const int height = 768;
    const int side = std::min<int>(width, height);
    glViewport((width - side) / 2, (height - side) / 2, side, side);
    g_shaderProgram->setUniform("modelView", mat4());
    g_shaderProgram->setUniform("projection", ortho(-0.4f, 1.4f, -0.4f, 1.4f, 0.f, 1.f));


    // Create and setup geometry
    auto vertexArray = std::vector<vec4>({
        vec4(0, 0, 0, 1)
      , vec4(1, 0, 0, 1)
      , vec4(0, 1, 0, 1)
      , vec4(1, 0, 0, 1)
      , vec4(0, 1, 0, 1)
      , vec4(1, 1, 0, 1) });

    auto colorArray = std::vector<vec4>({
        vec4(1, 0, 0, 1)
      , vec4(1, 1, 0, 1)
      , vec4(0, 0, 1, 1)
      , vec4(1, 1, 0, 1)
      , vec4(0, 0, 1, 1)
      , vec4(0, 1, 0, 1) });

    g_vertexBuffer1 = new Buffer();
    g_vertexBuffer1->ref();
    g_vertexBuffer1->setData(vertexArray, GL_STATIC_DRAW);
    g_vertexBuffer2 = new Buffer();
    g_vertexBuffer2->ref();
    g_vertexBuffer2->setData(vertexArray, GL_STATIC_DRAW);
    g_colorBuffer = new Buffer();
    g_colorBuffer->ref();
    g_colorBuffer->setData(colorArray, GL_STATIC_DRAW);

    g_vao = new VertexArray();
    g_vao->ref();

    g_vao->binding(0)->setAttribute(0);
    g_vao->binding(0)->setFormat(4, GL_FLOAT);

    g_vao->binding(1)->setAttribute(1);
    g_vao->binding(1)->setBuffer(g_colorBuffer, 0, sizeof(vec4));
    g_vao->binding(1)->setFormat(4, GL_FLOAT);

    g_vao->enable(0);
    g_vao->enable(1);


    // Create and setup TransformFeedback
    g_transformFeedback = new TransformFeedback();
    g_transformFeedback->ref();
    g_transformFeedback->setVaryings(g_transformFeedbackProgram, { { "next_position" } }, GL_INTERLEAVED_ATTRIBS);


    g_startTime = std::chrono::high_resolution_clock::now();
}

void deinitialize()
{
    g_vao->unref();
    g_transformFeedbackProgram->unref();
    g_transformFeedback->unref();
    g_shaderProgram->unref();
    g_vertexBuffer1->unref();
    g_vertexBuffer2->unref();
    g_colorBuffer->unref();
}

void draw()
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    const auto t_elapsed = std::chrono::high_resolution_clock::now() - g_startTime;
    g_startTime = std::chrono::high_resolution_clock::now();

    Buffer * drawBuffer  = g_vertexBuffer1;
    Buffer * writeBuffer = g_vertexBuffer2;

    g_vao->bind();

    g_transformFeedbackProgram->setUniform("deltaT", static_cast<float>(t_elapsed.count()) * float(std::nano::num) / float(std::nano::den));

    g_vao->binding(0)->setBuffer(drawBuffer, 0, sizeof(vec4));

    g_transformFeedback->bind();
    writeBuffer->bindBase(GL_TRANSFORM_FEEDBACK_BUFFER, 0);

    glEnable(GL_RASTERIZER_DISCARD);

    g_transformFeedbackProgram->use();
    g_transformFeedback->begin(GL_TRIANGLES);
    g_vao->drawArrays(GL_TRIANGLES, 0, 6);
    g_transformFeedback->end();
    glDisable(GL_RASTERIZER_DISCARD);

    g_transformFeedback->unbind();

    g_vao->binding(0)->setBuffer(writeBuffer, 0, sizeof(vec4));

    g_shaderProgram->use();
    g_transformFeedback->draw(GL_TRIANGLE_STRIP);
    g_shaderProgram->release();

    g_vao->unbind();

    std::swap(g_vertexBuffer1, g_vertexBuffer2);
}


int main(int /*argc*/, char * /*argv*/[])
{
    // Initialize GLFW
    glfwInit();

    GLFWwindow * window = createWindow();
    initialize();

    // Main loop
    while (!glfwWindowShouldClose(window))
    {
        glfwPollEvents();

        if (g_toggleFS)
        {
            deinitialize();
            destroyWindow(window);
            window = createWindow(!g_isFS);
            initialize();

            g_toggleFS = false;
        }

        draw();
        glfwSwapBuffers(window);
    }

    deinitialize();

    // Properly shutdown GLFW
    glfwTerminate();

    return 0;
}
