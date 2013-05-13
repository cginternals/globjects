#include <glow/Array.hpp>
#include <glow/ShaderFile.h>
#include <glow/Error.h>

#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/transform.hpp>

#include <Window.h>

Window::Window()
: shaderProgram(nullptr)
, vertexArrayObject(nullptr)
, texture(nullptr)
{
}

Window::~Window()
{
	delete shaderProgram;
	delete vertexArrayObject;
	delete texture;
}

void Window::initializeShaders(const std::string& applicationPath)
{
	std::string path = applicationPath.substr(0, applicationPath.rfind('/'));
	
	glow::ShaderFile* vertexShaderFile = new glow::ShaderFile(path+"/data/test.vert");
	glow::ShaderFile* fragmentShaderFile = new glow::ShaderFile(path+"/data/test.frag");

	glow::Shader* vertexShader = new glow::Shader(GL_VERTEX_SHADER);
	vertexShader->setSourceFile(vertexShaderFile, true);

	glow::Shader* fragmentShader = new glow::Shader(GL_FRAGMENT_SHADER);
	fragmentShader->setSourceFile(fragmentShaderFile, true);

	shaderProgram = new glow::Program();
	shaderProgram->attach(vertexShader);
	shaderProgram->attach(fragmentShader);
	shaderProgram->bindFragDataLocation(0, "outColor");
	shaderProgram->link();

	shaderProgram->setUniform("texture", 0);
}

void Window::initializeGL(const std::string& applicationPath)
{
	glClearColor(1,1,1,1);

	texture = new glow::Texture(GL_TEXTURE_2D);
	texture->setParameter(GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	texture->setParameter(GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	texture->image2D(0, GL_R32F, 512, 512, 0, GL_RED, GL_FLOAT, nullptr);

	initializeShaders(applicationPath);

	auto vertexArray = new glow::Vec3Array;
	*vertexArray << glm::vec3(0,0,0) << glm::vec3(1,0,0) << glm::vec3(1,1,0) << glm::vec3(0,1,0);

	auto texCoordArray = new glow::Vec2Array;
	*texCoordArray << glm::vec2(0,0) << glm::vec2(1,0) << glm::vec2(0,1) << glm::vec2(1,1);

	vertexArrayObject = new glow::VertexArrayObject();
	glow::Buffer* vertexBuffer = vertexArrayObject->addArrayBuffer("vertices");
	vertexBuffer->setData(vertexArray, GL_STATIC_DRAW);

	shaderProgram->attribute("position")->enable();
	shaderProgram->attribute("position")->setBuffer(vertexBuffer);

	glow::Buffer* texCoordsBuffer = vertexArrayObject->addArrayBuffer("texCoords");
	texCoordsBuffer->setData(texCoordArray, GL_STATIC_DRAW);

	shaderProgram->attribute("texCoord0")->enable();
	shaderProgram->attribute("texCoord0")->setBuffer(texCoordsBuffer);
}

void Window::resizeGL(int width, int height)
{
	int side = std::min(width, height);
	glViewport((width - side) / 2, (height - side) / 2, side, side);

	projection = glm::mat4();
	modelView = glm::ortho(0, 1, 0, 1, 0, 1);
}

void Window::paintGL()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	shaderProgram->setUniform("modelView", modelView);
	shaderProgram->setUniform("projection", projection);

	texture->bind();
	shaderProgram->use();

	vertexArrayObject->buffer("vertices")->drawArrays(GL_TRIANGLE_FAN, 0, 4);

        shaderProgram->release();
	texture->unbind();
}
