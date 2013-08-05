#include <glow/Array.hpp>
#include <glow/ShaderFile.h>
#include <glow/Error.h>

#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/transform.hpp>

#include <SSBOWindow.h>

SSBOWindow::SSBOWindow(const std::string& name, int x, int y, int w, int h)
: GlutWindow(name, x, y, w, h)
{
}

SSBOWindow::~SSBOWindow()
{
}

void SSBOWindow::initialize()
{
	glewExperimental = GL_TRUE;
	glewInit();
	glow::Error::clear();

	glClearColor(1,1,1,1);

	loadShaders();
	createGeometry();
	createSSBO();

	setupUniforms();
}

void SSBOWindow::loadShaders()
{
	glow::Shader* vertexShader = glow::Shader::fromFile(GL_VERTEX_SHADER, "shaders/ssbotest.vert");
	glow::Shader* fragmentShader = glow::Shader::fromFile(GL_FRAGMENT_SHADER, "shaders/ssbotest.frag");

	shaderProgram = new glow::Program();
	shaderProgram->attach(vertexShader, fragmentShader);
	shaderProgram->bindFragDataLocation(0, "outColor");
}

void SSBOWindow::createGeometry()
{
	glow::Vec3Array vertexArray;
	vertexArray << glm::vec3(0,0,0) << glm::vec3(1,0,0) << glm::vec3(1,1,0) << glm::vec3(0,1,0);

	glow::Vec2Array texCoordArray;
	texCoordArray << glm::vec2(0,1) << glm::vec2(1,1) << glm::vec2(1,0) << glm::vec2(0,0);

	vertexArrayObject = new glow::VertexArrayObject();

	vertexBuffer = new glow::Buffer(GL_ARRAY_BUFFER);
	vertexBuffer->setData(vertexArray);

	textureBuffer = new glow::Buffer(GL_ARRAY_BUFFER);
	textureBuffer->setData(texCoordArray);

	auto binding = vertexArrayObject->binding(0);
	binding->setBuffer(vertexBuffer, 0, sizeof(glm::vec3));
	binding->setFormat(3, GL_FLOAT);

	auto binding2 = vertexArrayObject->binding(1);
	binding2->setBuffer(textureBuffer, 0, sizeof(glm::vec2));
	binding2->setFormat(2, GL_FLOAT);

	vertexArrayObject->enable(shaderProgram->getAttributeLocation("position"));
	vertexArrayObject->enable(shaderProgram->getAttributeLocation("texCoord0"));
}

void SSBOWindow::createSSBO()
{
	int data[] = {
		1,2,3,4,5,6,7,8,9,10,
		10,1,2,3,4,5,6,7,8,9,
		9,10,1,2,3,4,5,6,7,8,
		8,9,10,1,2,3,4,5,6,7,
		7,8,9,10,1,2,3,4,5,6,
		6,7,8,9,10,1,2,3,4,5,
		5,6,7,8,9,10,1,2,3,4,
		4,5,6,7,8,9,10,1,2,3,
		3,4,5,6,7,8,9,10,1,2,
		2,3,4,5,6,7,8,9,10,1
	};

	ssbo = new glow::Buffer(GL_SHADER_STORAGE_BUFFER);
	ssbo->setData(sizeof(data), data, GL_DYNAMIC_DRAW);

	ssbo->bindBase(GL_SHADER_STORAGE_BUFFER, 1);
}

void SSBOWindow::setupUniforms()
{
	shaderProgram->setUniform("modelView", modelView);
	shaderProgram->setUniform("projection", projection);
	shaderProgram->setUniform("maximum", 10);
	shaderProgram->setUniform("rowCount", 10);
	shaderProgram->setUniform("columnCount", 10);
}

void SSBOWindow::reloadShaders()
{
	glow::ShaderFile::reloadAll();
}

void SSBOWindow::specialKeyEvent(int key, int x, int y)
{
	if (key == GLUT_KEY_F5)
	{
		reloadShaders();
		update();
	}
}

void SSBOWindow::resizeEvent(int width, int height)
{
	int side = std::min(width, height);
	glViewport((width - side) / 2, (height - side) / 2, side, side);

	projection = glm::mat4();
	modelView = glm::ortho(0, 1, 0, 1, 0, 10000);

	shaderProgram->setUniform("modelView", modelView);
	shaderProgram->setUniform("projection", projection);
}

void SSBOWindow::paint()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	shaderProgram->use();

	vertexArrayObject->bind();
	vertexBuffer->drawArrays(GL_TRIANGLE_FAN, 0, 4);
	vertexArrayObject->unbind();

        shaderProgram->release();
}
