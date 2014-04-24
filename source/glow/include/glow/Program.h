#pragma once

#include <set>
#include <unordered_map>
#include <vector>

#include <glm/glm.hpp>

#include <glow/glow_api.h>
#include <glow/Object.h>
#include <glow/ChangeListener.h>
#include <glow/ref_ptr.h>
#include <glow/LocationIdentity.h>
#include <glow/UniformBlock.h>

namespace glow
{
class Shader;
class ProgramBinary;

class AbstractUniform;
template<typename T> class Uniform;

/** \brief Wraps an OpenGL program.
    
    Therefor it suclasses Object. Programs get attached a set of shaders with 
    attach(). It inherits ChangeListener to react to changes to attached 
    shaders. To use a program for rendering, call use(). During use() the 
    program ensure that all attached shaders are compiled and linked. After 
    that, the program is registered in OpenGL to be used during the upcoming 
    rendering pileline calls.

    Shaders can be detached using detach() and queried with shaders().

    To use a program as a compute program, dispatchCompute() can be used to 
    start the kernel.

    Example code for setting up a program and use it for rendering
    
    \code{.cpp}

        Program * program = new Program();
        program->attach(
            Shader::fromString(GL_VERTEX_SHADER, "...")
          , Shader::fromString(GL_FRAGMENT_SHADER, "...")
          , ...);
        program->use();
    
        // draw calls
    
        program->release();

    \endcode
    
    Example code for using a program as compute program
    \code{.cpp}

        Program * program = new Program();
        program->attach(Shader::fromString(GL_COMPUTE_SHADER, "..."));
    
        program->dispatchCompute(128, 1, 1);
    
        program->release();

        \endcode
    
    \see http://www.opengl.org/wiki/Program_Object
    \see Shader
 */
class GLOW_API Program : public Object, protected ChangeListener
{
    friend class UniformBlock;
public:
	Program();
    Program(ProgramBinary * binary);

    virtual void accept(ObjectVisitor& visitor) override;

    void use() const;
    void release() const;

	bool isUsed() const;
	bool isLinked() const;

	//void attach(Shader * shader);
    template <class ...Shaders> 
    void attach(Shader * shader, Shaders... shaders);

	void detach(Shader * shader);

	std::set<Shader*> shaders() const;

    void link() const;
    void invalidate() const;

    void setBinary(ProgramBinary * binary);
    ProgramBinary * getBinary() const;

	const std::string infoLog() const;
	GLint get(GLenum pname) const;

    GLint getAttributeLocation(const std::string & name) const;
    GLint getUniformLocation(const std::string & name) const;

    std::vector<GLint> getAttributeLocations(const std::vector<std::string> & names) const;
    std::vector<GLint> getUniformLocations(const std::vector<std::string> & names) const;

    void bindAttributeLocation(GLuint index, const std::string & name) const;
    void bindFragDataLocation(GLuint index, const std::string & name) const;

    GLuint getResourceIndex(GLenum programInterface, const std::string & name) const;

    GLuint getUniformBlockIndex(const std::string& name) const;
    UniformBlock * uniformBlock(GLuint uniformBlockIndex);
    const UniformBlock * uniformBlock(GLuint uniformBlockIndex) const;
    UniformBlock * uniformBlock(const std::string& name);
    const UniformBlock * uniformBlock(const std::string& name) const;
    void getActiveUniforms(GLsizei uniformCount, const GLuint * uniformIndices, GLenum pname, GLint * params) const;
    std::vector<GLint> getActiveUniforms(const std::vector<GLuint> & uniformIndices, GLenum pname) const;
    std::vector<GLint> getActiveUniforms(const std::vector<GLint> & uniformIndices, GLenum pname) const;
    GLint getActiveUniform(GLuint uniformIndex, GLenum pname) const;
    std::string getActiveUniformName(GLuint uniformIndex) const;

	template<typename T>
	void setUniform(const std::string & name, const T & value);
    template<typename T>
    void setUniform(GLint location, const T & value);

	/** Retrieves the existing or creates a new typed uniform, named <name>.
	*/
	template<typename T>
	Uniform<T> * getUniform(const std::string & name);
    template<typename T>
    const Uniform<T> * getUniform(const std::string & name) const;
    template<typename T>
    Uniform<T> * getUniform(GLint location);
    template<typename T>
    const Uniform<T> * getUniform(GLint location) const;

	/** Adds the uniform to the internal list of named uniforms. If an equally
		named uniform already exists, this program derigisters itself and the uniform
		gets replaced (and by this the old one gets dereferenced). If the current
		program is linked, the uniforms value will be passed to the program object.
	*/
	void addUniform(AbstractUniform * uniform);

    void setShaderStorageBlockBinding(GLuint storageBlockIndex, GLuint storageBlockBinding) const;

	void dispatchCompute(GLuint numGroupsX, GLuint numGroupsY, GLuint numGroupsZ);
    void dispatchCompute(const glm::uvec3 & numGroups);
    void dispatchComputeGroupSize(GLuint numGroupsX, GLuint numGroupsY, GLuint numGroupsZ, GLuint groupSizeX, GLuint groupSizeY, GLuint groupSizeZ);
    void dispatchComputeGroupSize(const glm::uvec3 & numGroups, const glm::uvec3 & groupSizes);
protected:
    virtual ~Program();

    void attach();

    bool checkLinkStatus() const;
    void checkDirty() const;

    bool prepareForLinkage() const;
    bool compileAttachedShaders() const;
    void updateUniforms() const;
    void updateUniformBlockBindings() const;

	// ChangeListener Interface

    virtual void notifyChanged(const Changeable * sender) override;

protected:
	static GLuint createProgram();

    template<typename T>
    void setUniformByIdentity(const LocationIdentity & identity, const T & value);
    template<typename T>
    Uniform<T> * getUniformByIdentity(const LocationIdentity & identity);
    template<typename T>
    const Uniform<T> * getUniformByIdentity(const LocationIdentity & identity) const;

    UniformBlock * getUniformBlockByIdentity(const LocationIdentity & identity);
    const UniformBlock * getUniformBlockByIdentity(const LocationIdentity & identity) const;

protected:
	std::set<ref_ptr<Shader>> m_shaders;
    ref_ptr<ProgramBinary> m_binary;
    std::unordered_map<LocationIdentity, ref_ptr<AbstractUniform>> m_uniforms;
    std::unordered_map<LocationIdentity, UniformBlock> m_uniformBlocks;

    mutable bool m_linked;
    mutable bool m_dirty;
};

} // namespace glow

#include <glow/Program.hpp>
