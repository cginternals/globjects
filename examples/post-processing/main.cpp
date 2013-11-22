
#include <GL/glew.h>

#include <algorithm>
#include <random>
#include <cassert>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/transform.hpp>

#include <glow/Error.h>
#include <glow/Uniform.h>
#include <glow/Program.h>
#include <glow/Shader.h>
#include <glow/Buffer.h>
#include <glow/logging.h>
#include <glow/AutoTimer.h>
#include <glow/VertexArrayObject.h>
#include <glow/FrameBufferObject.h>
#include <glow/RenderBufferObject.h>
#include <glow/Texture.h>
#include <glow/Timer.h>

#include <glowutils/ScreenAlignedQuad.h>
#include <glowutils/MathMacros.h>
#include <glowutils/Icosahedron.h>
#include <glowutils/UnitCube.h>
#include <glowutils/AdaptiveGrid.h>
#include <glowutils/Camera.h>
#include <glowutils/FileRegistry.h>
#include <glowutils/File.h>

#include <glowwindow/ContextFormat.h>
#include <glowwindow/Context.h>
#include <glowwindow/Window.h>
#include <glowwindow/WindowEventHandler.h>

using namespace glowwindow;
using namespace glm;


class EventHandler : public WindowEventHandler
{
public:
	EventHandler()
    :   m_camera(vec3(0.f, 1.f, 4.0f))
	{
	}

	virtual ~EventHandler()
	{
	}

	void createAndSetupTexture();
	void createAndSetupShaders();
	void createAndSetupGeometry();

    virtual void initialize(Window & window) override
	{
        glow::DebugMessageOutput::enable();

		glClearColor(1.0f, 1.0f, 1.0f, 0.f);

		m_fbo = new glow::FrameBufferObject();

		m_normal = new glow::Texture(GL_TEXTURE_2D);
		m_normal->setParameter(GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		m_normal->setParameter(GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		m_normal->setParameter(GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		m_normal->setParameter(GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
		m_normal->setParameter(GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);

		m_geom = new glow::Texture(GL_TEXTURE_2D);
		m_geom->setParameter(GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		m_geom->setParameter(GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		m_geom->setParameter(GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		m_geom->setParameter(GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
		m_geom->setParameter(GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);

		m_depth = new glow::RenderBufferObject();

		m_fbo->attachTexture2D(GL_COLOR_ATTACHMENT0, m_normal);
		m_fbo->attachTexture2D(GL_COLOR_ATTACHMENT1, m_geom);
		m_fbo->attachRenderBuffer(GL_DEPTH_ATTACHMENT, m_depth);

		// ToDo: this could be done automatically by default..
		m_fbo->setDrawBuffers({ GL_COLOR_ATTACHMENT0, GL_COLOR_ATTACHMENT1 });


		m_sphere = new glow::Program();
		m_sphere->attach(
            glowutils::createShaderFromFile(GL_VERTEX_SHADER, "data/post-processing/sphere.vert")
        ,   glowutils::createShaderFromFile(GL_FRAGMENT_SHADER, "data/post-processing/sphere.frag"));

		m_phong = new glow::Program();
		m_phong->attach(
            glowutils::createShaderFromFile(GL_VERTEX_SHADER, "data/post-processing/phong.vert")
        ,	glowutils::createShaderFromFile(GL_FRAGMENT_SHADER, "data/post-processing/phong.frag"));

        m_icosahedron = new glowutils::Icosahedron(2);
        m_agrid = new glowutils::AdaptiveGrid(16U);

        m_quad = new glowutils::ScreenAlignedQuad(m_phong);

		m_time.reset();
		m_time.start();

        m_camera.setZNear(0.1f);
        m_camera.setZFar(16.f);

        m_agrid->setCamera(&m_camera);
	}

    virtual void resizeEvent(ResizeEvent & event) override
	{
        int width = event.width();
        int height = event.height();

        glViewport(0, 0, width, height);
        m_camera.setViewport(width, height);

		m_normal->image2D(0, GL_RGBA32F, width, height, 0, GL_RGBA, GL_FLOAT, nullptr);
		m_geom->image2D(0, GL_RGBA32F, width, height, 0, GL_RGBA, GL_FLOAT, nullptr);

        int result = glow::FrameBufferObject::defaultFBO()->getAttachmentParameter(GL_DEPTH, GL_FRAMEBUFFER_ATTACHMENT_DEPTH_SIZE);
        m_depth->storage(result == 16 ? GL_DEPTH_COMPONENT16 : GL_DEPTH_COMPONENT, width, height);
	}

    virtual void paintEvent(PaintEvent &) override
	{
        m_agrid->update();

        m_sphere->setUniform("transform", m_camera.viewProjection());
		m_sphere->setUniform("timef", mod(static_cast<float>(m_time.elapsed() * 1e-10), 1.f));

        m_phong->setUniform("transformi", m_camera.viewProjectionInverted());


		m_fbo->bind();
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		m_sphere->use();
		m_icosahedron->draw();
		m_sphere->release();

		m_fbo->unbind();

		glDisable(GL_DEPTH_TEST);
		glDepthMask(GL_FALSE);

		m_phong->setUniform("normal", 0);
		m_phong->setUniform("geom", 1);
        m_normal->bind(GL_TEXTURE0);
        m_geom->bind(GL_TEXTURE1);

		m_quad->draw();

        m_geom->unbind(GL_TEXTURE1);
        m_normal->unbind(GL_TEXTURE0);

		glEnable(GL_DEPTH_TEST);
		glDepthMask(GL_TRUE);

		// use the fbo's depth buffer as default depth buffer ;)
		// Note: this requires the depth formats to match exactly.

		glBindFramebuffer(GL_READ_FRAMEBUFFER, m_fbo->id());
		glBindFramebuffer(GL_DRAW_FRAMEBUFFER, 0);
        glBlitFramebuffer(0, 0, m_camera.viewport().x, m_camera.viewport().y, 0, 0, m_camera.viewport().x, m_camera.viewport().y,
			GL_DEPTH_BUFFER_BIT, GL_NEAREST);

		m_agrid->draw();
		
	}
    virtual void idle(Window & window) override
    {
        window.repaint();
    }

    virtual void keyPressEvent(KeyEvent & event) override
	{
		switch (event.key())
		{
		case GLFW_KEY_F5:
            glowutils::FileRegistry::instance().reloadAll();
			break;
		}
	}

protected:
    glow::ref_ptr<glow::Program> m_sphere;
    glow::ref_ptr<glow::Program> m_phong;

    glow::ref_ptr<glowutils::Icosahedron> m_icosahedron;
    glow::ref_ptr<glowutils::AdaptiveGrid> m_agrid;

    glow::ref_ptr<glowutils::ScreenAlignedQuad> m_quad;

    glow::ref_ptr<glow::FrameBufferObject> m_fbo;
    glow::ref_ptr<glow::Texture> m_normal;
    glow::ref_ptr<glow::Texture> m_geom;
    glow::ref_ptr<glow::RenderBufferObject> m_depth;

    glowutils::Camera m_camera;
	glow::Timer m_time;
};


/** This example shows ... .
*/
int main(int argc, char* argv[])
{
	ContextFormat format;
    format.setVersion(3, 0);
    format.setDepthBufferSize(16);

	Window window;

	window.setEventHandler(new EventHandler());

    if (window.create(format, "Post Processing Example"))
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
