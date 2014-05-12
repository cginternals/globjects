
#include <glbinding/functions.h>

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
#include <glow/VertexArrayObject.h>
#include <glow/FrameBufferObject.h>
#include <glow/RenderBufferObject.h>
#include <glow/Texture.h>
#include <glow/debugmessageoutput.h>

#include <glowutils/Timer.h>
#include <glowutils/ScreenAlignedQuad.h>
#include <glowutils/Icosahedron.h>
#include <glowutils/UnitCube.h>
#include <glowutils/AdaptiveGrid.h>
#include <glowutils/Camera.h>
#include <glowutils/File.h>
#include <glowutils/File.h>
#include <glowutils/glowutils.h>
#include <glowutils/StringTemplate.h>

#include <glowwindow/ContextFormat.h>
#include <glowwindow/Context.h>
#include <glowwindow/Window.h>
#include <glowwindow/WindowEventHandler.h>
#include <glowwindow/events.h>

#include <ExampleWindowEventHandler.h>

using namespace glowwindow;
using namespace glm;


class EventHandler : public ExampleWindowEventHandler
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
        ExampleWindowEventHandler::initialize(window);

        glow::debugmessageoutput::enable();

		gl::ClearColor(1.0f, 1.0f, 1.0f, 0.f);


		m_fbo = new glow::FrameBufferObject();

        m_normal = glow::Texture::createDefault(gl::TEXTURE_2D);
        m_geom = glow::Texture::createDefault(gl::TEXTURE_2D);

		m_depth = new glow::RenderBufferObject();

		m_fbo->attachTexture2D(gl::COLOR_ATTACHMENT0, m_normal);
		m_fbo->attachTexture2D(gl::COLOR_ATTACHMENT1, m_geom);
		m_fbo->attachRenderBuffer(gl::DEPTH_ATTACHMENT, m_depth);

        m_fbo->bind();
		// ToDo: this could be done automatically by default..
		m_fbo->setDrawBuffers({ gl::COLOR_ATTACHMENT0, gl::COLOR_ATTACHMENT1 });

                
        glowutils::StringTemplate* sphereVertexShader = new glowutils::StringTemplate(new glowutils::File("data/post-processing/sphere.vert"));
        glowutils::StringTemplate* sphereFragmentShader = new glowutils::StringTemplate(new glowutils::File("data/post-processing/sphere.frag"));
        glowutils::StringTemplate* phongVertexShader = new glowutils::StringTemplate(new glowutils::File("data/post-processing/phong.vert"));
        glowutils::StringTemplate* phongFragmentShader = new glowutils::StringTemplate(new glowutils::File("data/post-processing/phong.frag"));

#ifdef MAC_OS
                sphereVertexShader->replace("#version 140", "#version 150");
                sphereFragmentShader->replace("#version 140", "#version 150");
                phongVertexShader->replace("#version 140", "#version 150");
                phongFragmentShader->replace("#version 140", "#version 150");
#endif

                
		m_sphere = new glow::Program();
        m_sphere->attach(new glow::Shader(gl::VERTEX_SHADER, sphereVertexShader), new glow::Shader(gl::FRAGMENT_SHADER, sphereFragmentShader));
                

		m_phong = new glow::Program();
        m_phong->attach(new glow::Shader(gl::VERTEX_SHADER, phongVertexShader), new glow::Shader(gl::FRAGMENT_SHADER, phongFragmentShader));
                
                

        m_icosahedron = new glowutils::Icosahedron(2);
        m_agrid = new glowutils::AdaptiveGrid(16U);

        m_quad = new glowutils::ScreenAlignedQuad(m_phong);

		m_time.reset();
		m_time.start();

        m_camera.setZNear(0.1f);
        m_camera.setZFar(16.f);

        m_agrid->setCamera(&m_camera);
	}

    virtual void framebufferResizeEvent(ResizeEvent & event) override
	{
        int width = event.width();
        int height = event.height();

        gl::Viewport(0, 0, width, height);


        m_camera.setViewport(width, height);

		m_normal->image2D(0, gl::RGBA32F, width, height, 0, gl::RGBA, gl::FLOAT, nullptr);
		m_geom->image2D(0, gl::RGBA32F, width, height, 0, gl::RGBA, gl::FLOAT, nullptr);

        int result = glow::FrameBufferObject::defaultFBO()->getAttachmentParameter(gl::DEPTH, gl::FRAMEBUFFER_ATTACHMENT_DEPTH_SIZE);
        m_depth->storage(result == 16 ? gl::DEPTH_COMPONENT16 : gl::DEPTH_COMPONENT, width, height);
	}

    virtual void paintEvent(PaintEvent &) override
	{
        m_agrid->update();

        m_sphere->setUniform("transform", m_camera.viewProjection());
        m_sphere->setUniform("timef", mod(static_cast<float>(m_time.elapsed().count()) * 1e-10f, 1.f));

        m_phong->setUniform("transformi", m_camera.viewProjectionInverted());


		m_fbo->bind();
		gl::Clear(gl::COLOR_BUFFER_BIT | gl::DEPTH_BUFFER_BIT);


		m_sphere->use();
		m_icosahedron->draw();
		m_sphere->release();

		m_fbo->unbind();

		gl::Disable(gl::DEPTH_TEST);

        gl::DepthMask(gl::FALSE_);


		m_phong->setUniform("normal", 0);
		m_phong->setUniform("geom", 1);
        m_normal->bindActive(gl::TEXTURE0);
        m_geom->bindActive(gl::TEXTURE1);

		m_quad->draw();

        m_geom->unbindActive(gl::TEXTURE1);
        m_normal->unbindActive(gl::TEXTURE0);

		gl::Enable(gl::DEPTH_TEST);

        gl::DepthMask(gl::TRUE_);


		// use the fbo's depth buffer as default depth buffer ;)
		// Note: this requires the depth formats to match exactly.

		gl::BindFramebuffer(gl::READ_FRAMEBUFFER, m_fbo->id());
		gl::BindFramebuffer(gl::DRAW_FRAMEBUFFER, 0);
        gl::BlitFramebuffer(0, 0, m_camera.viewport().x, m_camera.viewport().y, 0, 0, m_camera.viewport().x, m_camera.viewport().y,
			gl::DEPTH_BUFFER_BIT, gl::NEAREST);

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
            glowutils::File::reloadAll();
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
    glowutils::Timer m_time;
};


/** This example shows ... .
*/
int main(int /*argc*/, char* /*argv*/[])
{
    glow::info() << "Usage:";
    glow::info() << "\t" << "ESC" << "\t\t" << "Close example";
    glow::info() << "\t" << "ALT + Enter" << "\t" << "Toggle fullscreen";
    glow::info() << "\t" << "F11" << "\t\t" << "Toggle fullscreen";
    glow::info() << "\t" << "F5" << "\t\t" << "Reload shaders";

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
