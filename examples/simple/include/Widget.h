#pragma once

#include <glow/Program.h>
#include <glow/Texture.h>
#include <glow/VertexArrayObject.h>

#include <QGLWidget>

class Widget : public QGLWidget
{
	Q_OBJECT
public:
	Widget(QWidget* parent = nullptr);
	~Widget();

	QSize sizeHint() const;
protected:
	static QGLFormat createFormat();

	virtual void initializeGL();
	virtual void resizeGL(int width, int height);
	virtual void paintGL();

	void initializeShaders();
protected:
	glow::Texture* texture;
	glow::Program* shaderProgram;
	glow::VertexArrayObject* vertexArrayObject;
	glm::mat4 projection;
	glm::mat4 modelView;
};

