#include <glow/Array.hpp>
#include <glow/ShaderFile.h>
#include <glow/Error.h>

#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/transform.hpp>

#include <CSWindow.h>

CSWindow::CSWindow(const std::string& name, int x, int y, int w, int h)
: GlutWindow(name, x, y, w, h)
, frame(0)
{
}

CSWindow::~CSWindow()
{
}

void CSWindow::initialize()
{
	glewExperimental = GL_TRUE;
	glewInit();
	glow::Error::clear();

	glClearColor(1,1,1,1);

	loadShaders();
	createGeometry();
	createTexture();

	setupUniforms();

	registerTimer(5, [this]() { step(); }, true);
}

void CSWindow::loadShaders()
{
	glow::Shader* vertexShader = glow::Shader::fromFile(GL_VERTEX_SHADER, "shaders/cstest.vert");
	glow::Shader* fragmentShader = glow::Shader::fromFile(GL_FRAGMENT_SHADER, "shaders/cstest.frag");

	shaderProgram = new glow::Program();
	shaderProgram->attach(vertexShader, fragmentShader);
	shaderProgram->bindFragDataLocation(0, "outColor");

	glow::Shader* computeShader = glow::Shader::fromFile(GL_COMPUTE_SHADER, "shaders/cstest.comp");

	computeProgram = new glow::Program();
	computeProgram->attach(computeShader);
}

void CSWindow::createGeometry()
{
	glow::Vec3Array vertexArray;
	vertexArray << glm::vec3(0,0,0) << glm::vec3(1,0,0) << glm::vec3(1,1,0) << glm::vec3(0,1,0);

	glow::Vec2Array texCoordArray;
	texCoordArray << glm::vec2(0,0) << glm::vec2(1,0) << glm::vec2(1,1) << glm::vec2(0,1);

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

void CSWindow::createTexture()
{
	texture = new glow::Texture(GL_TEXTURE_2D);
	texture->setParameter(GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	texture->setParameter(GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	texture->image2D(0, GL_R32F, 512, 512, 0, GL_RED, GL_FLOAT, nullptr);
	texture->bindImageTexture(0, 0, GL_FALSE, 0, GL_WRITE_ONLY, GL_R32F);
}

void CSWindow::setupUniforms()
{
	shaderProgram->setUniform("texture", 0);

	shaderProgram->setUniform("modelView", modelView);
	shaderProgram->setUniform("projection", projection);

	computeProgram->setUniform("roll", (float)frame*0.01f);
	computeProgram->setUniform("destTex", 0);
}

void CSWindow::step()
{
	frame = (frame + 1) % 628;

	computeProgram->setUniform("roll", (float)frame*0.01f);

	texture->bind();

	computeProgram->use();
	glDispatchCompute(512/16, 512/16, 1); // 512^2 threads in blocks of 16^2
	computeProgram->release();

	texture->unbind();

	update();
}

void CSWindow::reloadShaders()
{
	glow::ShaderFile::reloadAll();
}

void CSWindow::specialKeyEvent(int key, int x, int y)
{
	if (key == GLUT_KEY_F5)
	{
		reloadShaders();
		update();
	}
}

void CSWindow::resizeEvent(int width, int height)
{
	int side = std::min(width, height);
	glViewport((width - side) / 2, (height - side) / 2, side, side);

	projection = glm::mat4();
	modelView = glm::ortho(0, 1, 0, 1, 0, 10000);

	shaderProgram->setUniform("modelView", modelView);
	shaderProgram->setUniform("projection", projection);
}

void CSWindow::paint()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	texture->bind();

	shaderProgram->use();

	vertexArrayObject->bind();
	vertexBuffer->drawArrays(GL_TRIANGLE_FAN, 0, 4);
	vertexArrayObject->unbind();

        shaderProgram->release();

	texture->unbind();
}
