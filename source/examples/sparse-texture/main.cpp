
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


const static ivec2 s_textureSize(4096);
const static int s_maxResidentPages(512);


namespace {
    bool toggleFS = false;
    bool isFS = false;
}


void key_callback(GLFWwindow * window, int key, int /*scancode*/, int action, int /*modes*/)
{
    if (key == GLFW_KEY_ESCAPE && action == GLFW_RELEASE)
        glfwSetWindowShouldClose(window, true);

    if (key == GLFW_KEY_F5 && action == GLFW_RELEASE)
        File::reloadAll();

    if (key == GLFW_KEY_F11 && action == GLFW_RELEASE)
        toggleFS = true;
}

GLFWwindow * initialize(bool fs = false)
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
    if (!toggleFS)
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

    isFS = fs;
    return window;
}

void deinitialize(GLFWwindow * window)
{
    globjects::detachAllObjects();
    glfwDestroyWindow(window);
}

void mapNextPage(Texture * texture, ivec2 pageSize, ivec2 numPages, int totalPages)
{
    static int currentPage = 0;

    // create random texture data
    std::vector<unsigned char> data(pageSize.x * pageSize.y * 4);

    std::random_device rd;
    std::mt19937 generator(rd());

    std::poisson_distribution<> r(0.2);

    for (int i = 0; i < pageSize.x * pageSize.y * 4; ++i)
        data[i] = static_cast<unsigned char>(255 - static_cast<unsigned char>(r(generator) * 255));

    // unmap oldest page
    int oldestPage = (currentPage + totalPages - s_maxResidentPages) % totalPages;
    ivec2 oldOffset = ivec2(oldestPage % numPages.x, oldestPage / numPages.x) * pageSize;
    texture->pageCommitment(0, ivec3(oldOffset, 0), ivec3(pageSize, 1), GL_FALSE);

    // map next page
    ivec2 newOffset = ivec2(currentPage % numPages.x, currentPage / numPages.x) * pageSize;

    texture->pageCommitment(0, ivec3(newOffset, 0), ivec3(pageSize, 1), GL_TRUE);
    texture->subImage2D(0, newOffset, pageSize, GL_RGBA, GL_UNSIGNED_BYTE, data.data());

    currentPage = (currentPage + 1) % totalPages;
}

void draw(Texture * texture, ScreenAlignedQuad * quad, ivec2 pageSize, ivec2 numPages,int totalPages)
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    mapNextPage(texture, pageSize, numPages, totalPages);

    quad->draw();
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

    GLFWwindow * window = nullptr;

    do
    {
        // Deinitialize old window before fullscreen toggle
        if (window != nullptr) deinitialize(window);

        // Initialize window
        window = initialize(toggleFS ? !isFS : isFS);
        toggleFS = false;

        // Initialize OpenGL objects
        int numPageSizes;
        glGetInternalformativ(GL_TEXTURE_2D, GL_RGBA8, GL_NUM_VIRTUAL_PAGE_SIZES_ARB, sizeof(int), &numPageSizes);
        info("GL_NUM_VIRTUAL_PAGE_SIZES_ARB = %d;", numPageSizes);

        if (numPageSizes == 0) 
        {
            fatal("Sparse Texture not supported for GL_RGBA8");
            return 1;
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

        ivec2 pageSize   = ivec2(pageSizesX[0], pageSizesY[0]);
        ivec2 numPages   = s_textureSize / pageSize;
        int totalPages = numPages.x * numPages.y;

        // Get maximum sparse texture size

        int maxSparseTextureSize;
        glGetIntegerv(GL_MAX_SPARSE_TEXTURE_SIZE_ARB, &maxSparseTextureSize);
        info("GL_MAX_SPARSE_TEXTURE_SIZE_ARB = %d;", maxSparseTextureSize);

        ref_ptr<Texture> texture = new Texture(GL_TEXTURE_2D);

        // make texture sparse
        texture->setParameter(GL_TEXTURE_SPARSE_ARB, static_cast<GLint>(GL_TRUE));
        // specify the page size via its index in the array retrieved above (we simply use the first here)
        texture->setParameter(GL_VIRTUAL_PAGE_SIZE_INDEX_ARB, 0);

        texture->setParameter(GL_TEXTURE_MIN_FILTER, static_cast<GLint>(GL_LINEAR));
        texture->setParameter(GL_TEXTURE_MAG_FILTER, static_cast<GLint>(GL_LINEAR));

        texture->setParameter(GL_TEXTURE_WRAP_S, static_cast<GLint>(GL_CLAMP_TO_EDGE));
        texture->setParameter(GL_TEXTURE_WRAP_T, static_cast<GLint>(GL_CLAMP_TO_EDGE));
        texture->setParameter(GL_TEXTURE_WRAP_R, static_cast<GLint>(GL_CLAMP_TO_EDGE));

        // allocate virtual(!) storage for texture
        texture->storage2D(1, GL_RGBA8, s_textureSize);


        // Create and setup geometry
        ref_ptr<ScreenAlignedQuad> quad = new ScreenAlignedQuad(texture);
        quad->setSamplerUniform(0);



        // Main loop
        while (!toggleFS && !glfwWindowShouldClose(window))
        {
            glfwPollEvents();
            draw(texture, quad, pageSize, numPages, totalPages);
            glfwSwapBuffers(window);
        }

    }
    while (!glfwWindowShouldClose(window));

    // Properly shutdown GLFW
    glfwTerminate();

    return 0;
}
