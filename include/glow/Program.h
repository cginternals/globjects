#pragma once

#include <glow/Shader.h>

#include <glm/glm.hpp>

#include <set>
#include <map>

// http://www.opengl.org/wiki/Program_Object

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

	void link();

	void invalidate();

	std::string infoLog() const;

	GLint getAttributeLocation(const std::string& name);
	GLint getUniformLocation(const std::string& name);

	void bindAttributeLocation(GLuint index, const std::string& name);
	void bindFragDataLocation(GLuint index, const std::string& name);

	void setUniform(const std::string& name, int value);
	void setUniform(const std::string& name, float value);
	void setUniform(const std::string& name, const glm::mat4& value);
protected:
	std::set<ref_ptr<Shader>> _shaders;
	bool _linked;
	bool _dirty;

	void checkLinkStatus();
	void checkDirty();
};

} // namespace glow
