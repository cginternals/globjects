#pragma once

#include <glow/Program.h>
#include <glow/Texture.h>
#include <glow/VertexArrayObject.h>

class Window
{
public:
	Window();
	~Window();
	
	void initializeGL(const std::string& applicationPath);
	void resizeGL(int width, int height);
	void paintGL();
protected:
	void initializeShaders(const std::string& applicationPath);
	
	glow::Texture* texture;
	glow::Program* shaderProgram;
	glow::VertexArrayObject* vertexArrayObject;
	glm::mat4 projection;
	glm::mat4 modelView;
};