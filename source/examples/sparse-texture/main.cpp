
#include <algorithm>
#include <random>

#include <glm/glm.hpp>

#include <glbinding/gl/gl.h>
#include <glbinding/gl/extension.h>
#include <glbinding/ContextInfo.h>
#include <glbinding/Version.h>

#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>

#include <globjects/globjects.h>
#include <globjects/base/File.h>
#include <globjects/logging.h>

#include <globjects/Texture.h>

#include "ScreenAlignedQuad.h"


using namespace gl;
using namespace glm;
using namespace globjects;


namespace {
    bool g_toggleFS = false;
    bool g_isFS = false;

    Texture * g_texture = nullptr;
    ScreenAlignedQuad * g_quad = nullptr;
    ivec2 g_pageSize;
    ivec2 g_numPages;
    int g_totalPages;

    const ivec2 g_textureSize(4096);
    const int g_maxResidentPages(512);
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
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 1);
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

    if (!hasExtension(GLextension::GL_ARB_sparse_texture))
    {
        critical() << "Sparse textues not supported.";

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
    // Initialize OpenGL objects
    int numPageSizes;
    glGetInternalformativ(GL_TEXTURE_2D, GL_RGBA8, GL_NUM_VIRTUAL_PAGE_SIZES_ARB, sizeof(int), &numPageSizes);
    info("GL_NUM_VIRTUAL_PAGE_SIZES_ARB = %d;", numPageSizes);

    if (numPageSizes == 0) 
    {
        fatal("Sparse Texture not supported for GL_RGBA8");
        exit(1);
    }

    std::vector<int> pageSizesX(numPageSizes);
    glGetInternalformativ(GL_TEXTURE_2D, GL_RGBA8, GL_VIRTUAL_PAGE_SIZE_X_ARB
        , static_cast<GLsizei>(numPageSizes * sizeof(int)), pageSizesX.data());
    for (int i = 0; i < numPageSizes; ++i)
        info("GL_VIRTUAL_PAGE_SIZE_X_ARB[%;] = %;", i, pageSizesX[i]);

    std::vector<int> pageSizesY(numPageSizes);
    glGetInternalformativ(GL_TEXTURE_2D, GL_RGBA8, GL_VIRTUAL_PAGE_SIZE_Y_ARB
        , static_cast<GLsizei>(numPageSizes * sizeof(int)), pageSizesY.data());
    for (int i = 0; i < numPageSizes; ++i)
        info("GL_VIRTUAL_PAGE_SIZE_Y_ARB[%;] = %;", i, pageSizesY[i]);

    std::vector<int> pageSizesZ(numPageSizes);
    glGetInternalformativ(GL_TEXTURE_2D, GL_RGBA8, GL_VIRTUAL_PAGE_SIZE_Z_ARB
        , static_cast<GLsizei>(numPageSizes * sizeof(int)), pageSizesZ.data());
    for (int i = 0; i < numPageSizes; ++i)
        info("GL_VIRTUAL_PAGE_SIZE_Z_ARB[%;] = %;", i, pageSizesZ[i]);

    g_pageSize = ivec2(pageSizesX[0], pageSizesY[0]);
    g_numPages   = g_textureSize / g_pageSize;
    g_totalPages = g_numPages.x * g_numPages.y;

    // Get maximum sparse texture size

    int maxSparseTextureSize;
    glGetIntegerv(GL_MAX_SPARSE_TEXTURE_SIZE_ARB, &maxSparseTextureSize);
    info("GL_MAX_SPARSE_TEXTURE_SIZE_ARB = %d;", maxSparseTextureSize);

    g_texture = new Texture(GL_TEXTURE_2D);
    g_texture->ref();

    // make texture sparse
    g_texture->setParameter(GL_TEXTURE_SPARSE_ARB, static_cast<GLint>(GL_TRUE));
    // specify the page size via its index in the array retrieved above (we simply use the first here)
    g_texture->setParameter(GL_VIRTUAL_PAGE_SIZE_INDEX_ARB, 0);

    g_texture->setParameter(GL_TEXTURE_MIN_FILTER, static_cast<GLint>(GL_LINEAR));
    g_texture->setParameter(GL_TEXTURE_MAG_FILTER, static_cast<GLint>(GL_LINEAR));

    g_texture->setParameter(GL_TEXTURE_WRAP_S, static_cast<GLint>(GL_CLAMP_TO_EDGE));
    g_texture->setParameter(GL_TEXTURE_WRAP_T, static_cast<GLint>(GL_CLAMP_TO_EDGE));
    g_texture->setParameter(GL_TEXTURE_WRAP_R, static_cast<GLint>(GL_CLAMP_TO_EDGE));

    // allocate virtual(!) storage for texture
    g_texture->storage2D(1, GL_RGBA8, g_textureSize);


    // Create and setup geometry
    g_quad = new ScreenAlignedQuad(g_texture);
    g_quad->setSamplerUniform(0);
    g_quad->ref();
}

void deinitialize()
{
    g_texture->unref();
    g_quad->unref();
}

void mapNextPage()
{
    static int currentPage = 0;

    // create random texture data
    std::vector<unsigned char> data(g_pageSize.x * g_pageSize.y * 4);

    std::random_device rd;
    std::mt19937 generator(rd());

    std::poisson_distribution<> r(0.2);

    for (int i = 0; i < g_pageSize.x * g_pageSize.y * 4; ++i)
        data[i] = static_cast<unsigned char>(255 - static_cast<unsigned char>(r(generator) * 255));

    // unmap oldest page
    int oldestPage = (currentPage + g_totalPages - g_maxResidentPages) % g_totalPages;
    ivec2 oldOffset = ivec2(oldestPage % g_numPages.x, oldestPage / g_numPages.x) * g_pageSize;
    g_texture->pageCommitment(0, ivec3(oldOffset, 0), ivec3(g_pageSize, 1), GL_FALSE);

    // map next page
    ivec2 newOffset = ivec2(currentPage % g_numPages.x, currentPage / g_numPages.x) * g_pageSize;

    g_texture->pageCommitment(0, ivec3(newOffset, 0), ivec3(g_pageSize, 1), GL_TRUE);
    g_texture->subImage2D(0, newOffset, g_pageSize, GL_RGBA, GL_UNSIGNED_BYTE, data.data());

    currentPage = (currentPage + 1) % g_totalPages;
}

void draw()
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    mapNextPage();

    g_quad->draw();
}


/**
 * @brief This example shows how to set up a sparse texture and then map/unmap pages using the ARB_sparse_texture extension.
 *
 * See http://www.opengl.org/registry/specs/ARB/sparse_texture.txt
 */
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
