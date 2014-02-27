#include <GL/glew.h>

#include <algorithm>
#include <random>

#include <glow/Error.h>
#include <glow/logging.h>
#include <glow/Texture.h>
#include <glow/debugmessageoutput.h>

#include <glowutils/ScreenAlignedQuad.h>
#include <glowwindow/ContextFormat.h>
#include <glowwindow/Context.h>
#include <glowwindow/Window.h>
#include <glowwindow/WindowEventHandler.h>
#include <glowwindow/events.h>

#include <ExampleWindowEventHandler.h>

using namespace glowwindow;

class EventHandler : public ExampleWindowEventHandler
{
public:
    EventHandler()
    : m_textureSize(4096)
    , m_maxResidentPages(512)
    {
    }

    virtual ~EventHandler()
    {
    }

    void createAndSetupTexture();
	void createAndSetupGeometry();
    void mapNextPage();

    virtual void initialize(Window & ) override
    {
        glow::debugmessageoutput::enable();

        glClearColor(0.2f, 0.3f, 0.4f, 1.f);
        CheckGLError();

        createAndSetupTexture();
	    createAndSetupGeometry();
    }
    
    virtual void framebufferResizeEvent(ResizeEvent & event) override
    {
        int width = event.width();
        int height = event.height();
    	int side = std::min<int>(width, height);

        glViewport((width - side) / 2, (height - side) / 2, side, side);
        CheckGLError();
    }

    virtual void paintEvent(PaintEvent &) override
    {
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        CheckGLError();

        mapNextPage();

        m_quad->draw();
    }

    virtual void idle(Window & window) override
    {
        window.repaint();
    }

protected:
    glow::ref_ptr<glow::Texture> m_texture;
    glow::ref_ptr<glowutils::ScreenAlignedQuad> m_quad;
    glm::ivec2 m_textureSize;
    glm::ivec2 m_pageSize;
    glm::ivec2 m_numPages;
    int m_totalPages;
    int m_maxResidentPages;
};


/**
 * @brief This example shows how to set up a sparse texture and then map/unmap pages using the ARB_sparse_texture extension.
 *
 * See http://www.opengl.org/registry/specs/ARB/sparse_texture.txt
 */
int main(int /*argc*/, char* /*argv*/[])
{
    ContextFormat format;
    format.setVersion(3, 0);

    Window window;

    window.setEventHandler(new EventHandler());

    if (window.create(format, "Sparse Texture Example"))
    {
        window.context()->setSwapInterval(Context::VerticalSyncronization);

        window.show();

        return MainLoop::run();
    }
    else
    {
        return 1;
    }
}

void EventHandler::createAndSetupTexture()
{
    // Get available page sizes
    int numPageSizes;
    glGetInternalformativ(GL_TEXTURE_2D, GL_RGBA8, GL_NUM_VIRTUAL_PAGE_SIZES_ARB, sizeof(int), &numPageSizes);
    glow::info("GL_NUM_VIRTUAL_PAGE_SIZES_ARB = %d;", numPageSizes);
    if (numPageSizes == 0) {
        glow::fatal("Sparse Texture not supported for GL_RGBA8");
        return;
    }

    std::vector<int> pageSizesX(numPageSizes);
    glGetInternalformativ(GL_TEXTURE_2D, GL_RGBA8, GL_VIRTUAL_PAGE_SIZE_X_ARB, static_cast<GLsizei>(numPageSizes * sizeof(int)), pageSizesX.data());
    for (int i = 0; i < numPageSizes; ++i) {
        glow::info("GL_VIRTUAL_PAGE_SIZE_X_ARB[%;] = %;", i, pageSizesX[i]);
    }
    CheckGLError();

    std::vector<int> pageSizesY(numPageSizes);
    glGetInternalformativ(GL_TEXTURE_2D, GL_RGBA8, GL_VIRTUAL_PAGE_SIZE_Y_ARB, static_cast<GLsizei>(numPageSizes * sizeof(int)), pageSizesY.data());
    for (int i = 0; i < numPageSizes; ++i) {
        glow::info("GL_VIRTUAL_PAGE_SIZE_Y_ARB[%;] = %;", i, pageSizesY[i]);
    }
    CheckGLError();

    std::vector<int> pageSizesZ(numPageSizes);
    glGetInternalformativ(GL_TEXTURE_2D, GL_RGBA8, GL_VIRTUAL_PAGE_SIZE_Z_ARB, static_cast<GLsizei>(numPageSizes * sizeof(int)), pageSizesZ.data());
    for (int i = 0; i < numPageSizes; ++i) {
        glow::info("GL_VIRTUAL_PAGE_SIZE_Z_ARB[%;] = %;", i, pageSizesZ[i]);
    }
    CheckGLError();

    m_pageSize = glm::ivec2(pageSizesX[0], pageSizesY[0]);
    m_numPages = m_textureSize / m_pageSize;
    m_totalPages = m_numPages.x * m_numPages.y;

    // Get maximum sparse texture size
    int maxSparseTextureSize;
    glGetIntegerv(GL_MAX_SPARSE_TEXTURE_SIZE_ARB, &maxSparseTextureSize);
    glow::info("GL_MAX_SPARSE_TEXTURE_SIZE_ARB = %d;", maxSparseTextureSize);
    CheckGLError();

	m_texture = new glow::Texture(GL_TEXTURE_2D);

    // make texture sparse
    m_texture->setParameter(GL_TEXTURE_SPARSE_ARB, GL_TRUE);
    // specify the page size via its index in the array retrieved above (we simply use the first here)
    m_texture->setParameter(GL_VIRTUAL_PAGE_SIZE_INDEX_ARB, 0);

	m_texture->setParameter(GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	m_texture->setParameter(GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	m_texture->setParameter(GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	m_texture->setParameter(GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	m_texture->setParameter(GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);

    // allocate virtual(!) storage for texture
    m_texture->storage2D(1, GL_RGBA8, m_textureSize);
}

void EventHandler::createAndSetupGeometry()
{
	m_quad = new glowutils::ScreenAlignedQuad(m_texture);
    m_quad->setSamplerUniform(0);
}

void EventHandler::mapNextPage()
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
    glm::ivec2 oldOffset = glm::ivec2(oldestPage % m_numPages.x, oldestPage / m_numPages.x) * m_pageSize;
    m_texture->pageCommitment(0, glm::ivec3(oldOffset, 0), glm::ivec3(m_pageSize, 1), GL_FALSE);

    // map next page
    glm::ivec2 newOffset = glm::ivec2(currentPage % m_numPages.x, currentPage / m_numPages.x) * m_pageSize;
    m_texture->pageCommitment(0, glm::ivec3(newOffset, 0), glm::ivec3(m_pageSize, 1), GL_TRUE);
    m_texture->subImage2D(0, newOffset, m_pageSize, GL_RGBA, GL_UNSIGNED_BYTE, data.data());
    currentPage = (currentPage + 1) % m_totalPages;
}
