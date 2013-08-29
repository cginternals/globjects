#pragma once

#include <set>
#include <unordered_map>

#include <glm/glm.hpp>

#include <glow/glow.h>
#include <glow/Shader.h>
#include <glow/ChangeListener.h>

// http://www.opengl.org/wiki/Program_Object

namespace glow
{

class AbstractUniform;
template<typename T> class Uniform;

class GLOW_API Program : public Object, protected ChangeListener
{
public:
	Program();
	virtual ~Program();

	virtual void accept(ObjectVisitor& visitor);

	void use();
	void release();

	bool isUsed() const;
	bool isLinked() const;

	void attach(Shader * shader);
    Program & operator<<(Shader * shader);
	void detach(Shader * shader);

	std::set<Shader*> shaders() const;

	void link();
	void invalidate();

	const std::string infoLog() const;
	GLint get(GLenum pname) const;

	GLint getAttributeLocation(const std::string& name);
	GLint getUniformLocation(const std::string& name);

	void bindAttributeLocation(GLuint index, const std::string& name);
	void bindFragDataLocation(GLuint index, const std::string& name);

	GLuint getResourceIndex(GLenum programInterface, const std::string& name);


	template<typename T>
	void setUniform(const std::string & name, const T & value);

	/** Retrieves the existing or creates a new typed uniform, named <name>.
	*/
	template<typename T>
	Uniform<T> * getUniform(const std::string & name);

	/** Adds the uniform to the internal list of named uniforms. If an equally
		named uniform already exists, this program derigisters itself and the uniform
		gets replaced (and by this the old one gets dereferenced). If the current
		program is linked, the uniforms value will be passed to the program object.
	*/
	void addUniform(AbstractUniform * uniform);


	void setShaderStorageBlockBinding(GLuint storageBlockIndex, GLuint storageBlockBinding);

	void dispatchCompute(GLuint numGroupsX, GLuint numGroupsY, GLuint numGroupsZ);

protected:
	bool checkLinkStatus();
	void checkDirty();

	void updateUniforms();

	// ChangeListener Interface

	virtual void notifyChanged();

protected:
	static GLuint createProgram();

protected:
	std::set<ref_ptr<Shader>> m_shaders;
	std::unordered_map<std::string, ref_ptr<AbstractUniform>> m_uniforms;

	bool m_linked;
	bool m_dirty;
};

} // namespace glow

#include <glow/Program.hpp>
