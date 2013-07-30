#pragma once

#include <set>
#include <unordered_map>

#include <glm/glm.hpp>

#include <glow/glow.h>
#include <glow/Shader.h>
#include <glow/AbstractUniform.h>
#include <glow/ChangeListener.h>

// http://www.opengl.org/wiki/Program_Object

namespace glow 
{
template<typename T>
class Uniform;

class GLOW_API Program : public Object, protected ChangeListener
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

	template<typename T>
	void setUniform(const std::string & name, const T & value);
	template<typename T>
	Uniform<T> * getUniform(const std::string & name);

	void addUniform(AbstractUniform * uniform);

	void setShaderStorageBlockBinding(GLuint storageBlockIndex, GLuint storageBlockBinding);

	void dispatchCompute(GLuint numGroupsX, GLuint numGroupsY, GLuint numGroupsZ);
protected:
	std::set<ref_ptr<Shader>> _shaders;
	std::unordered_map<std::string, ref_ptr<AbstractUniform>> _uniforms;
	bool _linked;
	bool _dirty;

	void checkLinkStatus();
	void checkDirty();

	void updateUniforms();

	void notifyChanged();

	static GLuint createProgram();
public:
	// convenience
	void attach(Shader* shader1, Shader* shader2, Shader* shader3 = nullptr, Shader* shader4 = nullptr, Shader* shader5 = nullptr);
};

} // namespace glow

#include <glow/Program.hpp>