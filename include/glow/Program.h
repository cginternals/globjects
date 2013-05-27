#pragma once

#include "declspec.h"

#include <glow/Shader.h>
#include <glow/Uniform.h>

#include <glm/glm.hpp>

#include <set>
#include <unordered_map>

// http://www.opengl.org/wiki/Program_Object

namespace glow {

class GLOW_API Program : public Object
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

	GLuint getResourceIndex(GLenum programInterface, const std::string& name);

	void bindAttributeLocation(GLuint index, const std::string& name);
	void bindFragDataLocation(GLuint index, const std::string& name);

	Uniform* getUniform(const std::string& name);
	void addUniform(Uniform* uniform);
	template<typename... Rest> void addUniforms(Uniform* uniform, Rest... rest) { addUniform(uniform); addUniforms(rest...); }

	void setShaderStorageBlockBinding(GLuint storageBlockIndex, GLuint storageBlockBinding);
protected:
	std::set<ref_ptr<Shader>> _shaders;
	bool _linked;
	bool _dirty;
	std::unordered_map<std::string, ref_ptr<Uniform>> _uniforms;

	void checkLinkStatus();
	void checkDirty();
	void updateUniforms();

	void addUniforms() {}
};

} // namespace glow
