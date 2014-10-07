
#include <algorithm>
#include <random>

#include <glbinding/gl/gl.h>
#include <glbinding/gl/extension.h>

#include <globjects/globjects.h>
#include <globjects/logging.h>
#include <globjects/Texture.h>

#include <common/ScreenAlignedQuad.h>
#include <common/ContextFormat.h>
#include <common/Context.h>
#include <common/Window.h>
#include <common/WindowEventHandler.h>
#include <common/events.h>


using namespace gl;
using namespace glm;

class EventHandler : public WindowEventHandler
{
public:
    EventHandler()
    : m_textureSize(4096)
    , m_totalPages(0)
    , m_maxResidentPages(512)
    {
    }

    virtual ~EventHandler()
    {
    }

    virtual void initialize(Window & window) override
    {
        WindowEventHandler::initialize(window);

        if (!globjects::hasExtension(GLextension::GL_ARB_sparse_texture))
        {
            globjects::critical() << "Sparse textues not supported.";

            window.close();
            return;
        }

        glClearColor(0.2f, 0.3f, 0.4f, 1.f);

        createAndSetupTexture();
	    createAndSetupGeometry();
    }

    void createAndSetupTexture()
    {
        // Get available page sizes

        int numPageSizes;
        glGetInternalformativ(GL_TEXTURE_2D, GL_RGBA8, GL_NUM_VIRTUAL_PAGE_SIZES_ARB, sizeof(int), &numPageSizes);
        globjects::info("GL_NUM_VIRTUAL_PAGE_SIZES_ARB = %d;", numPageSizes);

        if (numPageSizes == 0) 
        {
            globjects::fatal("Sparse Texture not supported for GL_RGBA8");
            return;
        }

        std::vector<int> pageSizesX(numPageSizes);
        glGetInternalformativ(GL_TEXTURE_2D, GL_RGBA8, GL_VIRTUAL_PAGE_SIZE_X_ARB
            , static_cast<GLsizei>(numPageSizes * sizeof(int)), pageSizesX.data());
        for (int i = 0; i < numPageSizes; ++i)
            globjects::info("GL_VIRTUAL_PAGE_SIZE_X_ARB[%;] = %;", i, pageSizesX[i]);

        std::vector<int> pageSizesY(numPageSizes);
        glGetInternalformativ(GL_TEXTURE_2D, GL_RGBA8, GL_VIRTUAL_PAGE_SIZE_Y_ARB
            , static_cast<GLsizei>(numPageSizes * sizeof(int)), pageSizesY.data());
        for (int i = 0; i < numPageSizes; ++i)
            globjects::info("GL_VIRTUAL_PAGE_SIZE_Y_ARB[%;] = %;", i, pageSizesY[i]);

        std::vector<int> pageSizesZ(numPageSizes);
        glGetInternalformativ(GL_TEXTURE_2D, GL_RGBA8, GL_VIRTUAL_PAGE_SIZE_Z_ARB
            , static_cast<GLsizei>(numPageSizes * sizeof(int)), pageSizesZ.data());
        for (int i = 0; i < numPageSizes; ++i)
            globjects::info("GL_VIRTUAL_PAGE_SIZE_Z_ARB[%;] = %;", i, pageSizesZ[i]);

        m_pageSize   = ivec2(pageSizesX[0], pageSizesY[0]);
        m_numPages   = m_textureSize / m_pageSize;
        m_totalPages = m_numPages.x * m_numPages.y;

        // Get maximum sparse texture size

        int maxSparseTextureSize;
        glGetIntegerv(GL_MAX_SPARSE_TEXTURE_SIZE_ARB, &maxSparseTextureSize);
        globjects::info("GL_MAX_SPARSE_TEXTURE_SIZE_ARB = %d;", maxSparseTextureSize);

        m_texture = new globjects::Texture(GL_TEXTURE_2D);

        // make texture sparse
        m_texture->setParameter(GL_TEXTURE_SPARSE_ARB, static_cast<GLint>(GL_TRUE));
        // specify the page size via its index in the array retrieved above (we simply use the first here)
        m_texture->setParameter(GL_VIRTUAL_PAGE_SIZE_INDEX_ARB, 0);

        m_texture->setParameter(GL_TEXTURE_MIN_FILTER, static_cast<GLint>(GL_LINEAR));
        m_texture->setParameter(GL_TEXTURE_MAG_FILTER, static_cast<GLint>(GL_LINEAR));

        m_texture->setParameter(GL_TEXTURE_WRAP_S, static_cast<GLint>(GL_CLAMP_TO_EDGE));
        m_texture->setParameter(GL_TEXTURE_WRAP_T, static_cast<GLint>(GL_CLAMP_TO_EDGE));
        m_texture->setParameter(GL_TEXTURE_WRAP_R, static_cast<GLint>(GL_CLAMP_TO_EDGE));

        // allocate virtual(!) storage for texture
        m_texture->storage2D(1, GL_RGBA8, m_textureSize);
    }

    void createAndSetupGeometry()
    {
        m_quad = new ScreenAlignedQuad(m_texture);
        m_quad->setSamplerUniform(0);
    }

    void mapNextPage()
    {
        static int currentPage = 0;

        // create random texture data
        std::vector<unsigned char> data(m_pageSize.x * m_pageSize.y * 4);

        std::random_device rd;
        std::mt19937 generator(rd());

        std::poisson_distribution<> r(0.2);

        for (int i = 0; i < m_pageSize.x * m_pageSize.y * 4; ++i)
            data[i] = static_cast<unsigned char>(255 - static_cast<unsigned char>(r(generator) * 255));

        // unmap oldest page
        int oldestPage = (currentPage + m_totalPages - m_maxResidentPages) % m_totalPages;
        ivec2 oldOffset = ivec2(oldestPage % m_numPages.x, oldestPage / m_numPages.x) * m_pageSize;
        m_texture->pageCommitment(0, ivec3(oldOffset, 0), ivec3(m_pageSize, 1), GL_FALSE);

        // map next page
        ivec2 newOffset = ivec2(currentPage % m_numPages.x, currentPage / m_numPages.x) * m_pageSize;

        m_texture->pageCommitment(0, ivec3(newOffset, 0), ivec3(m_pageSize, 1), GL_TRUE);
        m_texture->subImage2D(0, newOffset, m_pageSize, GL_RGBA, GL_UNSIGNED_BYTE, data.data());

        currentPage = (currentPage + 1) % m_totalPages;
    }

    virtual void paintEvent(PaintEvent & event) override
    {
        WindowEventHandler::paintEvent(event);

        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        mapNextPage();

        m_quad->draw();
    }

protected:
    globjects::ref_ptr<globjects::Texture> m_texture;
    globjects::ref_ptr<ScreenAlignedQuad> m_quad;

    ivec2 m_textureSize;
    ivec2 m_pageSize;
    ivec2 m_numPages;

    int m_totalPages;
    int m_maxResidentPages;
};


/**
 * @brief This example shows how to set up a sparse texture and then map/unmap pages using the ARB_sparse_texture extension.
 *
 * See http://www.opengl.org/registry/specs/ARB/sparse_texture.txt
 */
int main(int /*argc*/, char * /*argv*/[])
{
    globjects::info() << "Usage:";
    globjects::info() << "\t" << "ESC" << "\t\t"       << "Close example";
    globjects::info() << "\t" << "ALT + Enter" << "\t" << "Toggle fullscreen";
    globjects::info() << "\t" << "F11" << "\t\t"       << "Toggle fullscreen";

    ContextFormat format;
    format.setVersion(3, 0);

    Window window;
    window.setEventHandler(new EventHandler());

    if (!window.create(format, "Sparse Texture Example"))
        return 1;

    window.context()->setSwapInterval(Context::VerticalSyncronization);
    window.show();

    return MainLoop::run();
}
