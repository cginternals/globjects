
#include <glbinding/gl/gl.h>

#include <glm/glm.hpp>
#include <glm/gtx/transform.hpp>

#include <globjects/Program.h>
#include <globjects/Texture.h>
#include <globjects/DebugMessage.h>

#include <globjects-window/ContextFormat.h>
#include <globjects-window/Window.h>
#include <globjects-window/MainLoop.h>
#include <globjects-window/Context.h>
#include <globjects-window/WindowEventHandler.h>
#include <globjects-window/events.h>

#include <globjects-utils/UnitCube.h>
#include <globjects-base/File.h>
#include <globjects-utils/Camera.h>
#include <globjects-utils/AbstractCoordinateProvider.h>
#include <globjects-utils/WorldInHandNavigation.h>
#include <globjects-base/File.h>
#include <globjects-utils/ScreenAlignedQuad.h>
#include "GlBlendAlgorithm.h"
#include "ABufferAlgorithm.h"
#include "WeightedAverageAlgorithm.h"
#include "HybridAlgorithm.h"
#include <globjects-utils/globjects-utils.h>

#include <ExampleWindowEventHandler.h>

namespace {

struct CubeUniformAttributes {
    glm::vec3 position;
    glm::vec4 color;

};

} // anonymous namespace

class EventHandler : public ExampleWindowEventHandler, gloutils::AbstractCoordinateProvider {
private:
	gloutils::Camera* m_camera;
	gloutils::UnitCube* m_cube;
	gloutils::WorldInHandNavigation m_nav;
	gloutils::AxisAlignedBoundingBox m_aabb;
	gloutils::ScreenAlignedQuad* m_quad;
    std::vector<AbstractTransparencyAlgorithm*> m_algos;
	
public:
    virtual void initialize(glowindow::Window & window) override {

		window.addTimer(0, 0);

        ExampleWindowEventHandler::initialize(window);

        glo::DebugMessage::enable();

        gl::glClearColor(1.0f, 1.0f, 1.0f, 1.0f);


		glo::Shader* vertexShader = glo::Shader::fromFile(gl::GL_VERTEX_SHADER, "data/transparency/transparency.vert");

        m_algos.push_back(new GlBlendAlgorithm);
        m_algos.push_back(new ABufferAlgorithm);
        m_algos.push_back(new WeightedAverageAlgorithm);
        m_algos.push_back(new HybridAlgorithm);
        for (auto& algo : m_algos) {
			algo->initialize("data/transparency/", vertexShader, nullptr);
        }

        m_cube = new gloutils::UnitCube;

        m_camera = new gloutils::Camera(glm::vec3(0.0f, 0.0f, -15.0f), glm::vec3(0.0f, 0.0f, -8.0f), glm::vec3(0.0f, 1.0f, 0.0f));

		// Setup the screen aligned quad stuff
		glo::Program* quadProgram = new glo::Program();
        quadProgram->attach(glo::Shader::fromFile(gl::GL_FRAGMENT_SHADER, "data/transparency/quad.frag"));
        quadProgram->attach(glo::Shader::fromFile(gl::GL_VERTEX_SHADER, "data/transparency/quad.vert"));
        m_quad = new gloutils::ScreenAlignedQuad(quadProgram);

		m_aabb.extend(glm::vec3(-1.f, -0.5f, -10.5f));
		m_aabb.extend(glm::vec3(0.f, 0.5f, -5.5));

		m_nav.setCamera(m_camera);
		m_nav.setCoordinateProvider(this);
        m_nav.setBoundaryHint(m_aabb);
	}

    virtual void paintEvent(glowindow::PaintEvent& event) override {
		int width = event.window()->width();
		int height = event.window()->height();

		const CubeUniformAttributes cubes[4] = {
            CubeUniformAttributes{ glm::vec3(0.0f, 0.0f, -10.0f), glm::vec4(1.0, 0.0, 0.0, 0.3) },
            CubeUniformAttributes{ glm::vec3(0.0f, -0.25f, -8.0f), glm::vec4(0.0, 1.0, 0.0, 0.3) },
            CubeUniformAttributes{ glm::vec3(0.0f, -0.5f, -6.0f), glm::vec4(0.0, 0.0, 1.0, 0.3) },
            CubeUniformAttributes{ glm::vec3(0.0f, -0.75f, -4.0f), glm::vec4(0.0, 1.0, 1.0, 1.0) }
		};

        for (auto& algo : m_algos) {
            algo->draw([&](glo::Program* program) {
                for (int c = 0; c < 4; c++) {
                    program->setUniform("modelmatrix", glm::translate<float>(cubes[c].position));
                    program->setUniform("color", cubes[c].color);
                    m_cube->draw();
                }
            }, m_camera, width, height);
        }

        // STAGE2 - Draw the texture of each algorithm& onto the screen aligned quad
		gl::glDisable(gl::GL_DEPTH_TEST);


		gl::glDepthMask(gl::GL_FALSE);


		m_quad->program()->setUniform("topLeft", 0);
        m_quad->program()->setUniform("topRight", 1);
        m_quad->program()->setUniform("bottomLeft", 2);
        m_quad->program()->setUniform("bottomRight", 3);

        for (unsigned int i = 0; i < std::min(size_t(4), m_algos.size()); ++i) {
            m_algos[i]->getOutput()->bindActive(gl::GL_TEXTURE0 + i);
        }

		m_quad->draw();

		gl::glEnable(gl::GL_DEPTH_TEST);


		gl::glDepthMask(gl::GL_TRUE);

	}

    virtual void framebufferResizeEvent(glowindow::ResizeEvent & event) override {
		int width = event.width();
		int height = event.height();

        gl::glViewport(0, 0, width, height);


        for (auto& algo : m_algos) {
            algo->resize(width, height);
        }		
	}

    virtual float depthAt(const glm::ivec2 & windowCoordinates) const override
	{
		return gloutils::AbstractCoordinateProvider::depthAt(*m_camera, gl::GL_DEPTH_COMPONENT, windowCoordinates);
	}

    virtual glm::vec3 objAt(const glm::ivec2 & windowCoordinates) const override
	{
		return unproject(*m_camera, static_cast<gl::GLenum>(gl::GL_DEPTH_COMPONENT), windowCoordinates);
	}

    virtual glm::vec3 objAt(const glm::ivec2 & windowCoordinates, const float depth) const override
	{
		return unproject(*m_camera, depth, windowCoordinates);
	}

    virtual glm::vec3 objAt(const glm::ivec2 & windowCoordinates, const float depth, const glm::mat4 & viewProjectionInverted) const override
	{
		return unproject(*m_camera, viewProjectionInverted, depth, windowCoordinates);
	}

	virtual void mousePressEvent(glowindow::MouseEvent & event) override
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

	virtual void mouseMoveEvent(glowindow::MouseEvent & event) override
	{
		switch (m_nav.mode())
		{
		case gloutils::WorldInHandNavigation::PanInteraction:
			m_nav.panProcess(event.pos());
			event.accept();
			break;

		case gloutils::WorldInHandNavigation::RotateInteraction:
			m_nav.rotateProcess(event.pos());
			event.accept();
            break;
        case gloutils::WorldInHandNavigation::NoInteraction:
            break;
		}
	}

	virtual void mouseReleaseEvent(glowindow::MouseEvent & event) override
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

	virtual void keyPressEvent(glowindow::KeyEvent & event) override
	{
        //const float d = 0.08f;

		switch (event.key())
		{
		case GLFW_KEY_F5:
			glo::File::reloadAll();
			break;
		}
	}

	virtual void timerEvent(glowindow::TimerEvent & event) override
	{
		event.window()->repaint();
	}

};

int main(int /*argc*/, char* /*argv*/[])
{
    glo::info() << "Usage:";
    glo::info() << "\t" << "ESC" << "\t\t" << "Close example";
    glo::info() << "\t" << "ALT + Enter" << "\t" << "Toggle fullscreen";
    glo::info() << "\t" << "F11" << "\t\t" << "Toggle fullscreen";
    glo::info() << "\t" << "F5" << "\t\t" << "Reload shaders";
    glo::info() << "\t" << "Left Mouse" << "\t" << "Pan scene";
    glo::info() << "\t" << "Right Mouse" << "\t" << "Rotate scene";

    glowindow::ContextFormat format;
    format.setVersion(4, 3);
    format.setDepthBufferSize(16);
    //format.setSamples(4);

    glowindow::Window window;

    if (!window.create(format, "Transparency")) return 1;
    window.context()->setSwapInterval(glowindow::Context::NoVerticalSyncronization);
    window.setEventHandler(new EventHandler());
    window.show();
    return glowindow::MainLoop::run();
}
