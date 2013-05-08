#include <GL/glew.h>

#include <glow/Array.hpp>
#include <glow/ShaderFile.h>
#include <glow/Error.h>

#include <glm/gtc/type_ptr.hpp>

#include <Widget.h>

#include <QApplication>
#include <QDebug>


Widget::Widget(QWidget* parent)
: QGLWidget(createFormat(), parent)
, shaderProgram(nullptr)
{
}

Widget::~Widget()
{
	delete shaderProgram;
	delete vertexArrayObject;
	delete texture;
}

QGLFormat Widget::createFormat()
{
	QGLFormat format;
	format.setVersion(4, 2);

	return format;
}

QSize Widget::sizeHint() const
{
	return QSize(800, 600);
}

void Widget::initializeShaders()
{
	QString path = QApplication::applicationDirPath();

	glow::ShaderFile* vertexShaderFile = new glow::ShaderFile((path+"/data/test.vert").toStdString());
	glow::ShaderFile* fragmentShaderFile = new glow::ShaderFile((path+"/data/test.frag").toStdString());

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

void Widget::initializeGL()
{
	glewExperimental = GL_TRUE;
	glewInit();

	glClearColor(1,1,1,1);

	texture = new glow::Texture(GL_TEXTURE_2D);
	texture->setParameter(GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	texture->setParameter(GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	texture->image2D(0, GL_R32F, 512, 512, 0, GL_RED, GL_FLOAT, nullptr);

	initializeShaders();

	auto vertexArray = new glow::Vec3Array;
	*vertexArray << glm::vec3(0,0,0) << glm::vec3(1,0,0) << glm::vec3(1,1,0) << glm::vec3(0,1,0);

	auto texCoordArray = new glow::Vec2Array;
	*texCoordArray << glm::vec2(0,0) << glm::vec2(1,0) << glm::vec2(0,1) << glm::vec2(1,1);

	vertexArrayObject = new glow::VertexArrayObject();
	glow::Buffer* vertexBuffer = vertexArrayObject->addArrayBuffer("vertices");
	vertexBuffer->data(vertexArray->byteSize(), vertexArray->rawData(), GL_STATIC_DRAW);
	vertexBuffer->vertexAttribPointer(shaderProgram->getAttributeLocation("position"), 3, GL_FLOAT);
	shaderProgram->enableVertexAttribArray("position");

//	vertexArrayObject = new glow::VertexArray();
//	glow::Buffer* vertexBuffer = vertexArrayObject->createArrayBuffer("vertices");
//	vertexBuffer->data(vertexArray, GL_STATIC_DRAW);
//	shaderProgram->vertexAttrib("position")->enable();
//	shaderProgram->vertexAttrib("position")->bindBuffer(vertexBuffer, 3, GL_FLOAT);

	glow::Buffer* texCoordsBuffer = vertexArrayObject->addArrayBuffer("texCoords");
	texCoordsBuffer->data(texCoordArray->byteSize(), texCoordArray->rawData(), GL_STATIC_DRAW);
	texCoordsBuffer->vertexAttribPointer(shaderProgram->getAttributeLocation("texCoord0"), 2, GL_FLOAT);
	shaderProgram->enableVertexAttribArray("texCoord0");
}

void Widget::resizeGL(int width, int height)
{
	int side = qMin(width, height);
	glViewport((width - side) / 2, (height - side) / 2, side, side);

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glOrtho(0,1,0,1,0,1);
	glMatrixMode(GL_MODELVIEW);
}

void Widget::paintGL()
{
	glm::mat4 modelViewMatrix;
	glm::mat4 projectionMatrix;
	glGetFloatv(GL_MODELVIEW_MATRIX, glm::value_ptr(modelViewMatrix));
	glGetFloatv(GL_PROJECTION_MATRIX, glm::value_ptr(projectionMatrix));

	shaderProgram->setUniform("modelView", modelViewMatrix);
	shaderProgram->setUniform("projection", projectionMatrix);

	// render

	texture->bind();
	shaderProgram->use();

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	vertexArrayObject->buffer("vertices")->drawArrays(GL_TRIANGLE_FAN, 0, 4);

        shaderProgram->release();
	texture->unbind();
}
