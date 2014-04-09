#pragma once

#include <vector>
#include <array>

#include <GL/glew.h>

#include <glow/glow_api.h>
#include <glow/Object.h>

namespace glow
{

class ObjectVisitor;
class Program;

/** \brief Interface to use transform feedback.
    
    The TransformFeedback methods map directly to their OpenGL function counterparts.
    The draw and setVaryings method additionally binds the TransformFeedback object,
    whereas the begin, pause, resume and end methods doesn't.
    
    An example usage:
    \code{.cpp}

        Program* program = createProgramIncludingShaders();
        TransformFeedback* transformFeedback = new TransformFeedback();
        Buffer* buffer = new glow::Buffer(GL_ARRAY_BUFFER);
    
        transformFeedback->setVaryings(program, Array<const char*>{ "vertex_out" }, GL_INTERLEAVED_ATTRIBS);
    
        buffer->bindBase(GL_TRANSFORM_FEEDBACK_BUFFER, 0);
        buffer->setData(vertexSize, nullptr, GL_DYNAMIC_DRAW);
    
        program->use(); // use Program before issueing the begin method of TransformFeedback
    
        transformFeedback->bind();
        transformFeedback->begin(GL_POINTS); // record point vertices
    
        // draw calls
    
        transformFeedback->end(); // stop recording vertices before releasing the Program
        transformFeedback->unbind();
    
        program->release();
    
        // replay recorded vertices
        transformFeeback->draw(GL_POINTS);
   
   \endcode
 
    \see http://www.opengl.org/registry/specs/EXT/transform_feedback.txt
    \see http://www.opengl.org/registry/specs/ARB/transform_feedback2.txt
    \see http://www.opengl.org/registry/specs/ARB/transform_feedback3.txt
*/
class GLOW_API TransformFeedback : public Object
{
public:
	TransformFeedback();
	virtual ~TransformFeedback();

    virtual void accept(ObjectVisitor & visitor) override;

    void bind() const;
    void unbind() const;

	void begin(GLenum primitiveMode);
	void pause();
	void resume();
	void end();

    void draw(GLenum primitiveMode) const;

	void setVaryings(
        const Program * program
    ,   GLsizei count
    ,   const char ** varyingNames
    ,   GLenum bufferMode) const;

    void setVaryings(const Program * program
    ,   const std::vector<const char *> &varyingNames
    ,   GLenum bufferMode) const;

    template <std::size_t Count>
    void setVaryings(const Program * program
    ,   const std::array<const char *, Count> &varyingNames
    ,   GLenum bufferMode) const;

	bool isTransformFeedback() const;

protected:
	static GLuint genTransformFeedback();
};

} // namespace glow

#include <glow/TransformFeedback.hpp>
