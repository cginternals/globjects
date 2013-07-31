
#include <CreateContext.h>
#include <algorithm>

#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/transform.hpp>

#include <glow/Uniform.h>
#include <glow/Array.hpp>
#include <glow/ShaderFile.h>
#include <glow/Error.h>
#include <glow/Log.h>


Vertex::Vertex()
{
}

Vertex::Vertex(const glm::vec3& pos, const glm::vec2& texCoord)
: position(pos)
, texCoord0(texCoord)
{
}

void Window::initializeGL()
{
	glClearColor(1,1,1,1);

	createTexture();
	createShaders();
	createVertices();
}

void Window::createTexture()
{
	unsigned char img_data[16*4] = {
		255,0,0,100,
		255,255,0,255,
		255,0,255,255,
		255,0,100,255,
		0,0,80,255,
		255,0,0,255,
		255,255,0,0,
		255,0,255,255,
		255,0,100,255,
		0,0,80,255,
		255,0,0,255,
		255,255,0,0,
		255,0,255,255,
		255,0,100,255,
		0,0,80,255,
		0,0,80,255
	};

	texture = new glow::Texture(GL_TEXTURE_2D);

	texture->setParameter(GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	texture->setParameter(GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	texture->setParameter(GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	texture->setParameter(GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	texture->setParameter(GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);

	texture->image2D(0, GL_RGBA8, 4, 4, 0, GL_RGBA, GL_UNSIGNED_BYTE, img_data);
}

void Window::createShaders()
{
	glow::Shader* vertexShader = glow::Shader::fromFile(GL_VERTEX_SHADER, "data/simple/test.vert");
	glow::Shader* fragmentShader = glow::Shader::fromFile(GL_FRAGMENT_SHADER, "data/simple/test.frag");

	shaderProgram = new glow::Program();
	shaderProgram->attach(vertexShader, fragmentShader);
	shaderProgram->bindFragDataLocation(0, "outColor");

	shaderProgram->getUniform<GLint>("texture")->set(0);
}

void Window::createVertices()
{
	glow::Array<Vertex> vertices;

	vertices
		<< Vertex(glm::vec3(0,0,0), glm::vec2(0,0))
		<< Vertex(glm::vec3(1,0,0), glm::vec2(1,0))
		<< Vertex(glm::vec3(1,1,0), glm::vec2(1,1))
		<< Vertex(glm::vec3(0,1,0), glm::vec2(0,1));

	vertexArrayObject = new glow::VertexArrayObject();

	vertexBuffer = new glow::Buffer(GL_ARRAY_BUFFER);
	vertexBuffer->setData(vertices);

	auto binding1 = vertexArrayObject->binding(0);
	binding1->setAttribute(shaderProgram->getAttributeLocation("position"));
	binding1->setBuffer(vertexBuffer, 0, sizeof(Vertex));
	binding1->setFormat(3, GL_FLOAT, GL_FALSE, offsetof(Vertex, position));

	auto binding2 = vertexArrayObject->binding(1);
	binding2->setAttribute(shaderProgram->getAttributeLocation("texCoord0"));
	binding2->setBuffer(vertexBuffer, 0, sizeof(Vertex));
	binding2->setFormat(2, GL_FLOAT, GL_FALSE, offsetof(Vertex, texCoord0));

	vertexArrayObject->enable(shaderProgram->getAttributeLocation("position"));
	vertexArrayObject->enable(shaderProgram->getAttributeLocation("texCoord0"));
}

void Window::resizeGL(int width, int height)
{
	int side = std::min(width, height);
	glViewport((width - side) / 2, (height - side) / 2, side, side);

	shaderProgram->getUniform<glm::mat4>("modelView")->set(glm::mat4());
	shaderProgram->getUniform<glm::mat4>("projection")->set(glm::ortho(0.f, 1.f, 0.f, 1.f, 0.f, 1.f));
}

void Window::paintGL()
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
