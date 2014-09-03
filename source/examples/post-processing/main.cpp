
#include <glbinding/gl/functions.h>

#include <algorithm>
#include <random>
#include <cassert>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/transform.hpp>

#include <glbinding/gl/gl.h>

#include <globjects/Uniform.h>
#include <globjects/Program.h>
#include <globjects/Shader.h>
#include <globjects/Buffer.h>
#include <globjects/logging.h>
#include <globjects/VertexArray.h>
#include <globjects/Framebuffer.h>
#include <globjects/Renderbuffer.h>
#include <globjects/Texture.h>
#include <globjects/DebugMessage.h>

#include <globjects-utils/Timer.h>
#include <globjects-utils/ScreenAlignedQuad.h>
#include <globjects-utils/Icosahedron.h>
#include <globjects-utils/UnitCube.h>
#include <globjects-utils/AdaptiveGrid.h>
#include <globjects-utils/Camera.h>
#include <globjects-base/File.h>
#include <globjects-base/File.h>
#include <globjects-utils/globjects-utils.h>
#include <globjects-utils/StringTemplate.h>

#include <common/ContextFormat.h>
#include <common/Context.h>
#include <common/Window.h>
#include <common/WindowEventHandler.h>
#include <common/events.h>

#include <ExampleWindowEventHandler.h>


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

        glo::DebugMessage::enable();

        gl::glClearColor(1.0f, 1.0f, 1.0f, 0.f);

        m_fbo = new glo::Framebuffer();

        m_normal = glo::Texture::createDefault(gl::GL_TEXTURE_2D);
        m_geom = glo::Texture::createDefault(gl::GL_TEXTURE_2D);

        m_depth = new glo::Renderbuffer();

        m_fbo->attachTexture(gl::GL_COLOR_ATTACHMENT0, m_normal);
        m_fbo->attachTexture(gl::GL_COLOR_ATTACHMENT1, m_geom);
        m_fbo->attachRenderBuffer(gl::GL_DEPTH_ATTACHMENT, m_depth);

        m_fbo->bind(gl::GL_FRAMEBUFFER);
        // ToDo: this could be done automatically by default..
        m_fbo->setDrawBuffers({ gl::GL_COLOR_ATTACHMENT0, gl::GL_COLOR_ATTACHMENT1 });

        gloutils::StringTemplate* sphereVertexShader = new gloutils::StringTemplate(new glo::File("data/post-processing/sphere.vert"));
        gloutils::StringTemplate* sphereFragmentShader = new gloutils::StringTemplate(new glo::File("data/post-processing/sphere.frag"));
        gloutils::StringTemplate* phongVertexShader = new gloutils::StringTemplate(new glo::File("data/post-processing/phong.vert"));
        gloutils::StringTemplate* phongFragmentShader = new gloutils::StringTemplate(new glo::File("data/post-processing/phong.frag"));

#ifdef MAC_OS
                sphereVertexShader->replace("#version 140", "#version 150");
                sphereFragmentShader->replace("#version 140", "#version 150");
                phongVertexShader->replace("#version 140", "#version 150");
                phongFragmentShader->replace("#version 140", "#version 150");
#endif

                
		m_sphere = new glo::Program();
        m_sphere->attach(new glo::Shader(gl::GL_VERTEX_SHADER, sphereVertexShader), new glo::Shader(gl::GL_FRAGMENT_SHADER, sphereFragmentShader));
                

		m_phong = new glo::Program();
        m_phong->attach(new glo::Shader(gl::GL_VERTEX_SHADER, phongVertexShader), new glo::Shader(gl::GL_FRAGMENT_SHADER, phongFragmentShader));
                
                

        m_icosahedron = new gloutils::Icosahedron(2);
        m_agrid = new gloutils::AdaptiveGrid(16U);

        m_quad = new gloutils::ScreenAlignedQuad(m_phong);

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

        gl::glViewport(0, 0, width, height);


        m_camera.setViewport(width, height);

		m_normal->image2D(0, gl::GL_RGBA32F, width, height, 0, gl::GL_RGBA, gl::GL_FLOAT, nullptr);
		m_geom->image2D(0, gl::GL_RGBA32F, width, height, 0, gl::GL_RGBA, gl::GL_FLOAT, nullptr);

        int result = glo::Framebuffer::defaultFBO()->getAttachmentParameter(gl::GL_DEPTH, gl::GL_FRAMEBUFFER_ATTACHMENT_DEPTH_SIZE);
        m_depth->storage(result == 16 ? gl::GL_DEPTH_COMPONENT16 : gl::GL_DEPTH_COMPONENT, width, height);
	}

    virtual void paintEvent(PaintEvent &) override
	{
        m_agrid->update();

        m_sphere->setUniform("transform", m_camera.viewProjection());
        m_sphere->setUniform("timef", mod(static_cast<float>(m_time.elapsed().count()) * 1e-10f, 1.f));

        m_phong->setUniform("transformi", m_camera.viewProjectionInverted());


        m_fbo->bind(gl::GL_FRAMEBUFFER);
		gl::glClear(gl::GL_COLOR_BUFFER_BIT | gl::GL_DEPTH_BUFFER_BIT);


		m_sphere->use();
		m_icosahedron->draw();
		m_sphere->release();

        m_fbo->unbind(gl::GL_FRAMEBUFFER);

		gl::glDisable(gl::GL_DEPTH_TEST);

        gl::glDepthMask(gl::GL_FALSE);


		m_phong->setUniform("normal", 0);
		m_phong->setUniform("geom", 1);
        m_normal->bindActive(gl::GL_TEXTURE0);
        m_geom->bindActive(gl::GL_TEXTURE1);

		m_quad->draw();

        m_geom->unbindActive(gl::GL_TEXTURE1);
        m_normal->unbindActive(gl::GL_TEXTURE0);

		gl::glEnable(gl::GL_DEPTH_TEST);

        gl::glDepthMask(gl::GL_TRUE);


		// use the fbo's depth buffer as default depth buffer ;)
		// Note: this requires the depth formats to match exactly.

		gl::glBindFramebuffer(gl::GL_READ_FRAMEBUFFER, m_fbo->id());
		gl::glBindFramebuffer(gl::GL_DRAW_FRAMEBUFFER, 0);
        gl::glBlitFramebuffer(0, 0, m_camera.viewport().x, m_camera.viewport().y, 0, 0, m_camera.viewport().x, m_camera.viewport().y,
			gl::GL_DEPTH_BUFFER_BIT, gl::GL_NEAREST);

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
            glo::File::reloadAll();
			break;
		}
	}

protected:
    glo::ref_ptr<glo::Program> m_sphere;
    glo::ref_ptr<glo::Program> m_phong;

    glo::ref_ptr<gloutils::Icosahedron> m_icosahedron;
    glo::ref_ptr<gloutils::AdaptiveGrid> m_agrid;

    glo::ref_ptr<gloutils::ScreenAlignedQuad> m_quad;

    glo::ref_ptr<glo::Framebuffer> m_fbo;
    glo::ref_ptr<glo::Texture> m_normal;
    glo::ref_ptr<glo::Texture> m_geom;
    glo::ref_ptr<glo::Renderbuffer> m_depth;

    gloutils::Camera m_camera;
    gloutils::Timer m_time;
};


/** This example shows ... .
*/
int main(int /*argc*/, char* /*argv*/[])
{
    glo::info() << "Usage:";
    glo::info() << "\t" << "ESC" << "\t\t" << "Close example";
    glo::info() << "\t" << "ALT + Enter" << "\t" << "Toggle fullscreen";
    glo::info() << "\t" << "F11" << "\t\t" << "Toggle fullscreen";
    glo::info() << "\t" << "F5" << "\t\t" << "Reload shaders";

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
