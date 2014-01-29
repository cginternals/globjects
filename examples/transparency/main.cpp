#include <GL/glew.h>

#include <glm/glm.hpp>
#include <glm/gtx/transform.hpp>

#include <glow/Program.h>
#include <glow/Texture.h>
#include <glow/debugmessageoutput.h>

#include <glowwindow/ContextFormat.h>
#include <glowwindow/Window.h>
#include <glowwindow/MainLoop.h>
#include <glowwindow/Context.h>
#include <glowwindow/WindowEventHandler.h>
#include <glowwindow/events.h>

#include <glowutils/UnitCube.h>
#include <glowutils/File.h>
#include <glowutils/Camera.h>
#include <glowutils/AbstractCoordinateProvider.h>
#include <glowutils/WorldInHandNavigation.h>
#include <glowutils/File.h>
#include <glowutils/ScreenAlignedQuad.h>
#include <glowutils/GlBlendAlgorithm.h>
#include <glowutils/ABufferAlgorithm.h>
#include <glowutils/WeightedAverageAlgorithm.h>
#include <glowutils/HybridAlgorithm.h>
#include <glowutils/global.h>

namespace {

struct CubeUniformAttributes {
    glm::vec3 position;
    glm::vec4 color;

};

} // anonymous namespace

class EventHandler : public glowwindow::WindowEventHandler, glowutils::AbstractCoordinateProvider {
private:
	glowutils::Camera* m_camera;
	glowutils::UnitCube* m_cube;
	glowutils::WorldInHandNavigation m_nav;
	glowutils::AxisAlignedBoundingBox m_aabb;
	glowutils::ScreenAlignedQuad* m_quad;
	std::vector<glowutils::AbstractTransparencyAlgorithm*> m_algos;
	
public:
    virtual void initialize(glowwindow::Window & window) override {

		window.addTimer(0, 0);

		glow::debugmessageoutput::enable();

        glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
        CheckGLError();

		glow::Shader* vertexShader = glowutils::createShaderFromFile(GL_VERTEX_SHADER, "data/transparency/transparency.vert");

		m_algos.push_back(new glowutils::GlBlendAlgorithm);
        m_algos.push_back(new glowutils::ABufferAlgorithm);
        m_algos.push_back(new glowutils::WeightedAverageAlgorithm);
        m_algos.push_back(new glowutils::HybridAlgorithm);
        for (auto& algo : m_algos) {
			algo->initialize("data/transparency/", vertexShader, nullptr);
        }

        m_cube = new glowutils::UnitCube;

        m_camera = new glowutils::Camera(glm::vec3(0.0f, 0.0f, -15.0f), glm::vec3(0.0f, 0.0f, -8.0f), glm::vec3(0.0f, 1.0f, 0.0f));

		// Setup the screen aligned quad stuff
		glow::Program* quadProgram = new glow::Program();
        quadProgram->attach(glowutils::createShaderFromFile(GL_FRAGMENT_SHADER, "data/transparency/quad.frag"));
        quadProgram->attach(glowutils::createShaderFromFile(GL_VERTEX_SHADER, "data/transparency/quad.vert"));
        m_quad = new glowutils::ScreenAlignedQuad(quadProgram);

		m_aabb.extend(glm::vec3(-1.f, -0.5f, -10.5f));
		m_aabb.extend(glm::vec3(0.f, 0.5f, -5.5));

		m_nav.setCamera(m_camera);
		m_nav.setCoordinateProvider(this);
        m_nav.setBoundaryHint(m_aabb);
	}

    virtual void paintEvent(glowwindow::PaintEvent& event) override {
		int width = event.window()->width();
		int height = event.window()->height();

		const CubeUniformAttributes cubes[4] = {
            CubeUniformAttributes{ glm::vec3(0.0f, 0.0f, -10.0f), glm::vec4(1.0, 0.0, 0.0, 0.3) },
            CubeUniformAttributes{ glm::vec3(0.0f, -0.25f, -8.0f), glm::vec4(0.0, 1.0, 0.0, 0.3) },
            CubeUniformAttributes{ glm::vec3(0.0f, -0.5f, -6.0f), glm::vec4(0.0, 0.0, 1.0, 0.3) },
            CubeUniformAttributes{ glm::vec3(0.0f, -0.75f, -4.0f), glm::vec4(0.0, 1.0, 1.0, 1.0) }
		};

        for (auto& algo : m_algos) {
            algo->draw([&](glow::Program* program) {
                for (int c = 0; c < 4; c++) {
                    program->setUniform("modelmatrix", glm::translate<float>(cubes[c].position));
                    program->setUniform("color", cubes[c].color);
                    m_cube->draw();
                }
            }, m_camera, width, height);
        }

        // STAGE2 - Draw the texture of each algorithm& onto the screen aligned quad
		glDisable(GL_DEPTH_TEST);
		CheckGLError();

		glDepthMask(GL_FALSE);
		CheckGLError();

		m_quad->program()->setUniform("topLeft", 0);
        m_quad->program()->setUniform("topRight", 1);
        m_quad->program()->setUniform("bottomLeft", 2);
        m_quad->program()->setUniform("bottomRight", 3);

        for (unsigned int i = 0; i < std::min(size_t(4), m_algos.size()); ++i) {
            m_algos[i]->getOutput()->bindActive(GL_TEXTURE0 + i);
        }

		m_quad->draw();

		glEnable(GL_DEPTH_TEST);
		CheckGLError();

		glDepthMask(GL_TRUE);
		CheckGLError();
	}

    virtual void framebufferResizeEvent(glowwindow::ResizeEvent & event) override {
		int width = event.width();
		int height = event.height();

        glViewport(0, 0, width, height);
        CheckGLError();

        for (auto& algo : m_algos) {
            algo->resize(width, height);
        }		
	}

    virtual float depthAt(const glm::ivec2 & windowCoordinates) override
	{
		return glowutils::AbstractCoordinateProvider::depthAt(*m_camera, GL_DEPTH_COMPONENT, windowCoordinates);
	}

    virtual glm::vec3 objAt(const glm::ivec2 & windowCoordinates) override
	{
		return unproject(*m_camera, static_cast<GLenum>(GL_DEPTH_COMPONENT), windowCoordinates);
	}

    virtual glm::vec3 objAt(const glm::ivec2 & windowCoordinates, const float depth) override
	{
		return unproject(*m_camera, depth, windowCoordinates);
	}

    virtual glm::vec3 objAt(const glm::ivec2 & windowCoordinates, const float depth, const glm::mat4 & viewProjectionInverted) override
	{
		return unproject(*m_camera, viewProjectionInverted, depth, windowCoordinates);
	}

	virtual void mousePressEvent(glowwindow::MouseEvent & event) override
	{
		switch (event.button())
		{
		case GLFW_MOUSE_BUTTON_LEFT:
			m_nav.panBegin(event.pos());
			event.accept();
			break;

		case GLFW_MOUSE_BUTTON_RIGHT:
			m_nav.rotateBegin(event.pos());
			event.accept();
			break;
		}
	}

	virtual void mouseMoveEvent(glowwindow::MouseEvent & event) override
	{
		switch (m_nav.mode())
		{
		case glowutils::WorldInHandNavigation::PanInteraction:
			m_nav.panProcess(event.pos());
			event.accept();
			break;

		case glowutils::WorldInHandNavigation::RotateInteraction:
			m_nav.rotateProcess(event.pos());
			event.accept();
            break;
        case glowutils::WorldInHandNavigation::NoInteraction:
            break;
		}
	}

	virtual void mouseReleaseEvent(glowwindow::MouseEvent & event) override
	{
		switch (event.button())
		{
		case GLFW_MOUSE_BUTTON_LEFT:
			m_nav.panEnd();
			event.accept();
			break;

		case GLFW_MOUSE_BUTTON_RIGHT:
			m_nav.rotateEnd();
			event.accept();
			break;
		}
	}

	virtual void keyPressEvent(glowwindow::KeyEvent & event) override
	{
        //const float d = 0.08f;

		switch (event.key())
		{
		case GLFW_KEY_F5:
			glowutils::File::reloadAll();
			break;
		}
	}

	virtual void timerEvent(glowwindow::TimerEvent & event) override
	{
		event.window()->repaint();
	}

};

int main(int /*argc*/, char* /*argv*/[])
{
	glowwindow::ContextFormat format;
	format.setVersion(4, 3);
	format.setDepthBufferSize(16);
	//format.setSamples(4);

	glowwindow::Window window;

	if (!window.create(format, "Transparency")) return 1;
    window.context()->setSwapInterval(glowwindow::Context::NoVerticalSyncronization);
	window.setEventHandler(new EventHandler());
	window.show();
	return glowwindow::MainLoop::run();
	
}
