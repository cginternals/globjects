#pragma once

#include <set>
#include <unordered_map>

#include <glm/glm.hpp>

#include <glow/glow.h>
#include <glow/Shader.h>
#include <glow/ChangeListener.h>

namespace glow
{

class AbstractUniform;
template<typename T> class Uniform;

/**
 * \brief The Program class wraps an OpenGL program.
 *
 * Therefor it suclasses Object.
 * Programs gets attached a set of shaders with attach().
 * It subclasses ChangeListener to react to changes to attached shaders.
 * To use a program for rendering, call use().
 * During use() the program ensure that all attached shaders are compiled and linked.
 * After that, the program is registered in OpenGL to be used during the upcoming rendering pileline calls.
 *
 * Shaders can be detached using detach() and queried with shaders().
 *
 * To use a program as a compute program, dispatchCompute() can be used to start the kernel.
 *
 * Example code for setting up a program and use it for rendering
 * \code
 * Program* program = new Program();
 * program->attach(Shader::fromString(GL_VERTEX_SHADER, "..."), Shader::fromString(GL_FRAGMENT_SHADER, "..."));
 * program->use();
 *
 * // draw calls
 *
 * program->release();
 * \endcode
 *
 * Example code for using a program as compute program
 * \code
 * Program* program = new Program();
 * program->attach(Shader::fromString(GL_COMPUTE_SHADER, "..."));
 *
 * program->dispatchCompute(128, 1, 1);
 *
 * program->release();
 * \endcode
 *
 * \see http://www.opengl.org/wiki/Program_Object
 * \see Shader
 */
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

	//void attach(Shader * shader);
    template <class ...Shaders> 
    void attach(Shader * shader, Shaders... shaders);

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
    void dispatchComputeGroupSize(GLuint numGroupsX, GLuint numGroupsY, GLuint numGroupsZ, GLuint groupSizeX, GLuint groupSizeY, GLuint groupSizeZ);
protected:
    void attach();

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
