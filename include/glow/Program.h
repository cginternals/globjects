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

	void use();
	void release();

	bool isUsed() const;
	bool isLinked() const;

	void attach(Shader* shader);
	void detach(Shader* shader);

	const std::vector<Shader*>& shaders() const;

	void link();

	void invalidate();

	std::string infoLog() const;

	GLint attributeLocation(const std::string& name);
	GLint uniformLocation(const std::string& name);

	void bindAttributeLocation(GLuint index, const std::string& name);
	void bindFragDataLocation(GLuint index, const std::string& name);

	void enableVertexAttribArray(GLint index);
	void disableVertexAttribArray(GLint index);
	void enableVertexAttribArray(const std::string& name);
	void disableVertexAttribArray(const std::string& name);

	void setUniform(const std::string& name, int value);
	void setUniform(const std::string& name, float value);
	void setUniform(const std::string& name, const glm::mat4& value);
protected:
	std::vector<Shader*> _shaders;
	bool _linked;
	bool _dirty;

	void checkLinkStatus();
	void checkDirty();
};

} // namespace glow
