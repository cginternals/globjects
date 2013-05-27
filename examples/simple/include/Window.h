#pragma once

#include <glow/Program.h>
#include <glow/Texture.h>
#include <glow/VertexArrayObject.h>

struct Vertex
{
	Vertex();
	Vertex(const glm::vec3& pos, const glm::vec2& texCoord);

	glm::vec3 position;
	glm::vec2 texCoord0;
};

class Window
{
public:
	void initializeGL();
	void resizeGL(int width, int height);
	void paintGL();
protected:
	void createTexture();
	void createShaders();
	void createVertices();

	glow::ref_ptr<glow::Texture> texture;
	glow::ref_ptr<glow::Program> shaderProgram;
	glow::ref_ptr<glow::VertexArrayObject> vertexArrayObject;
};
