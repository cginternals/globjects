
#include <GL/glew.h>

#include <algorithm>
#include <random>
#include <cassert>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/transform.hpp>

#include <glow/Error.h>
#include <glow/Uniform.h>
#include <glow/ShaderFile.h>
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
#include <glowutils/AxonometricLookAt.h>

#include <glowwindow/ContextFormat.h>
#include <glowwindow/Context.h>
#include <glowwindow/Window.h>
#include <glowwindow/WindowEventHandler.h>

using namespace glow;
using namespace glm;


class EventHandler : public WindowEventHandler
{
public:
	EventHandler()
		: m_iso(false)
	{
		m_eye = vec3(0.f, 1.f, 4.0f);
		m_center = vec3(0.f, 0.f, 0.f);
	}

	virtual ~EventHandler()
	{
	}

	void createAndSetupTexture();
	void createAndSetupShaders();
	void createAndSetupGeometry();

	virtual void initialize(Window & window)
	{
		glEnable(GL_TEXTURE_2D);

		DebugMessageOutput::enable();

		glClearColor(1.0f, 1.0f, 1.0f, 0.f);

		m_fbo = new FrameBufferObject();

		m_normal = new Texture(GL_TEXTURE_2D);
		m_normal->setParameter(GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		m_normal->setParameter(GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		m_normal->setParameter(GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		m_normal->setParameter(GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
		m_normal->setParameter(GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);

		m_geom = new Texture(GL_TEXTURE_2D);
		m_geom->setParameter(GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		m_geom->setParameter(GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		m_geom->setParameter(GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		m_geom->setParameter(GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
		m_geom->setParameter(GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);

		m_depth = new RenderBufferObject();

		m_fbo->attachTexture2D(GL_COLOR_ATTACHMENT0, m_normal);
		m_fbo->attachTexture2D(GL_COLOR_ATTACHMENT1, m_geom);
		m_fbo->attachRenderBuffer(GL_DEPTH_ATTACHMENT, m_depth);

		// ToDo: this could be done automatically by default..
		m_fbo->setDrawBuffers({ GL_COLOR_ATTACHMENT0, GL_COLOR_ATTACHMENT1 });


		m_sphere = new Program();
		m_sphere->attach(
			Shader::fromFile(GL_VERTEX_SHADER, "data/post-processing/sphere.vert")
			, Shader::fromFile(GL_FRAGMENT_SHADER, "data/post-processing/sphere.frag"));

		m_phong = new Program();
		m_phong->attach(
			Shader::fromFile(GL_VERTEX_SHADER, "data/post-processing/phong.vert")
		,	Shader::fromFile(GL_FRAGMENT_SHADER, "data/post-processing/phong.frag"));

		m_icosahedron = new Icosahedron(2);
		m_agrid = new AdaptiveGrid(16U);

		m_quad = new ScreenAlignedQuad(m_phong);

		m_time.reset();
		m_time.start();
	}

	virtual void resizeEvent(
		Window & window
		, unsigned int width
		, unsigned int height)
	{
		glViewport(0, 0, width, height);

		if (!m_sphere)
			return;

		m_size = ivec2(width, height);

		m_normal->image2D(0, GL_RGBA32F, width, height, 0, GL_RGBA, GL_FLOAT, nullptr);
		m_geom->image2D(0, GL_RGBA32F, width, height, 0, GL_RGBA, GL_FLOAT, nullptr);
		m_depth->storage(GL_DEPTH_COMPONENT, width, height);

		const float aspect(static_cast<float>(width) / static_cast<float>(height));
		m_projection = perspective(40.f, aspect, 0.1f, 1024.f);
		m_ortho = ortho(-aspect, aspect, -1.f, 1.f, 0.1f, 1024.f);

		m_agrid->setNearFar(0.1f, 1024.f);

		m_axonometric.setPosition(m_eye);
		const mat4 view(m_iso ? m_axonometric.matrix() : lookAt(m_eye, m_center, vec3(0.0, 1.0, 0.0)));


		m_sphere->setUniform("transform", (m_iso ? m_ortho : m_projection) * view);
		m_agrid->update(m_eye, (m_iso ? m_ortho : m_projection) * view);
	}

	virtual void paintEvent(Window & window)
	{
		const mat4 view(m_iso ? m_axonometric.matrix() : lookAt(m_eye, m_center, vec3(0.0, 1.0, 0.0)));

		m_agrid->update(m_eye, (m_iso ? m_ortho : m_projection) * view);
		m_sphere->setUniform("transform", (m_iso ? m_ortho : m_projection) * view);
		m_sphere->setUniform("timef", mod(static_cast<float>(m_time.elapsed() * 1e-10), 1.f));

		m_phong->setUniform("transformi", inverse((m_iso ? m_ortho : m_projection) * view));


		m_fbo->bind();
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		m_sphere->use();
		m_icosahedron->draw();
		m_sphere->release();

		m_fbo->unbind();

		glDisable(GL_DEPTH_TEST);
		glDepthMask(GL_FALSE);

		// ToDo: Simplify with glow
		m_phong->setUniform("normal", 0);
		m_phong->setUniform("geom", 1);
		glActiveTexture(GL_TEXTURE0 + 0);
		m_normal->bind();
		glActiveTexture(GL_TEXTURE0 + 1);
		m_geom->bind();


		m_quad->draw();

		glActiveTexture(GL_TEXTURE0);

		glEnable(GL_DEPTH_TEST);
		glDepthMask(GL_TRUE);

		// use the fbo's depth buffer as default depth buffer ;)
		// Note: this requires the depth formats to match exactly.

		glBindFramebuffer(GL_READ_FRAMEBUFFER, m_fbo->id());
		glBindFramebuffer(GL_DRAW_FRAMEBUFFER, 0);
		glBlitFramebuffer(0, 0, m_size.x, m_size.y, 0, 0, m_size.x, m_size.y,
			GL_DEPTH_BUFFER_BIT, GL_NEAREST);

		m_agrid->draw();
		
	}
	virtual void idleEvent(Window & window)
	{
		window.repaint();
	}

	virtual void keyPressEvent(
		Window & window
		, KeyEvent & event)
	{
		const float d = 0.08f;

		switch (event.key())
		{
		case GLFW_KEY_F5:
			glow::ShaderFile::reloadAll();
			break;
		case GLFW_KEY_SPACE:
			m_iso = !m_iso;
			break;
		case GLFW_KEY_1:
			m_center.x += d;
			m_eye.x += d;
			break;
		case GLFW_KEY_2:
			m_eye.x -= d;
			m_center.x -= d;
			break;
		case GLFW_KEY_3:
			m_eye += (m_eye - m_center) * 0.1f;
			break;
		case GLFW_KEY_4:
			m_eye -= (m_eye - m_center) * 0.1f;
			break;
		case GLFW_KEY_5:
			m_center.z += d;
			m_eye.z += d;
			break;
		case GLFW_KEY_6:
			m_center.z -= d;
			m_eye.z -= d;
			break;
		}

		m_axonometric.setPosition(m_eye);
	}

protected:
	ref_ptr<Program> m_sphere;
	ref_ptr<Program> m_phong;

	ref_ptr<Icosahedron> m_icosahedron;
	ref_ptr<AdaptiveGrid> m_agrid;

	ref_ptr<ScreenAlignedQuad> m_quad;

	ref_ptr<FrameBufferObject> m_fbo;
	ref_ptr<Texture> m_normal;
	ref_ptr<Texture> m_geom;
	ref_ptr<RenderBufferObject> m_depth;

	vec3 m_eye;
	vec3 m_center;

	mat4 m_projection;
	mat4 m_ortho;

	bool m_iso;

	ivec2 m_size;
	Timer m_time;

	AxonometricLookAt m_axonometric;
};


/** This example shows ... .
*/
int main(int argc, char** argv)
{
	glewExperimental = GL_TRUE;

	ContextFormat format;
	format.setVersion(4, 0);
	format.setProfile(ContextFormat::CoreProfile);

	Window window;
	window.assign(new EventHandler());

	window.create(format, "Post Processing Example");
	window.context()->setSwapInterval(Context::VerticalSyncronization);
	window.show();

	return Window::run();
}
