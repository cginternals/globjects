#pragma once

#include <GlutWindow.h>

#include <glow/Program.h>
#include <glow/Texture.h>
#include <glow/Buffer.h>
#include <glow/VertexArrayObject.h>
#include <glow/Array.hpp>
#include <glow/ref_ptr.hpp>

class SSBOWindow : public GlutWindow
{
public:
	SSBOWindow(const std::string& name, int x, int y, int w, int h);
	~SSBOWindow();

	virtual void initialize();
	void resizeEvent(int width, int height);
	void paint();
	void specialKeyEvent(int key, int x, int y);
protected:
	glow::ref_ptr<glow::Texture> texture;
	glow::ref_ptr<glow::Program> shaderProgram;
	glow::ref_ptr<glow::VertexArrayObject> vertexArrayObject;
	glow::ref_ptr<glow::Buffer> ssbo;
	glow::ref_ptr<glow::Buffer> vertexBuffer;
	glow::ref_ptr<glow::Buffer> textureBuffer;
	glm::mat4 projection;
	glm::mat4 modelView;
protected:
	void reloadShaders();
	void loadShaders();
	void createGeometry();
	void createSSBO();
	void setupUniforms();
};
