#pragma once

#include <glow/Shader.h>

#include <glm/glm.hpp>

#include <vector>

namespace glow {

class Program : public Object
{
public:
	Program();
	~Program();

	void bind();
	static void unbind();

	void attach(Shader* shader);
	void link();

	GLint getAttribLocation(const char* name);
	GLint getUniformLocation(const char* name);

	void bindAttribLocation(GLuint index, const char* name);
	void bindFragDataLocation(GLuint index, const char* name);

	void enableVertexAttribArray(GLint index);
	void disableVertexAttribArray(GLint index);
	void enableVertexAttribArray(const char* name);
	void disableVertexAttribArray(const char* name);

	void setUniform(const char* name, int value);
	void setUniform(const char* name, float value);
	void setUniform(const char* name, const glm::mat4& value);
protected:
	std::vector<Shader*> _shaders;

	void checkLinkStatus();
};

} // namespace glow
