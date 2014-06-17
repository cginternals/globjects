#pragma once

#include <vector>
#include <array>

#include <glbinding/types.h>
#include <glbinding/constants.h>

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
        Buffer* buffer = new Buffer(gl::GL_ARRAY_BUFFER);
    
        transformFeedback->setVaryings(program, Array<const char*>{ "vertex_out" }, gl::GL_INTERLEAVED_ATTRIBS);
    
        buffer->bindBase(gl::GL_TRANSFORM_FEEDBACK_BUFFER, 0);
        buffer->setData(vertexSize, nullptr, gl::GL_DYNAMIC_DRAW);
    
        program->use(); // use Program before issueing the begin method of TransformFeedback
    
        transformFeedback->bind();
        transformFeedback->begin(gl::GL_POINTS); // record point vertices
    
        // draw calls
    
        transformFeedback->end(); // stop recording vertices before releasing the Program
        transformFeedback->unbind();
    
        program->release();
    
        // replay recorded vertices
        transformFeeback->draw(gl::GL_POINTS);
   
   \endcode
 
    \see http://www.opengl.org/registry/specs/EXT/transform_feedback.txt
    \see http://www.opengl.org/registry/specs/ARB/transform_feedback2.txt
    \see http://www.opengl.org/registry/specs/ARB/transform_feedback3.txt
*/
class GLOW_API TransformFeedback : public Object
{
public:
	TransformFeedback();

    virtual void accept(ObjectVisitor & visitor) override;

    void bind(gl::GLenum target = gl::GL_TRANSFORM_FEEDBACK) const;
    static void unbind(gl::GLenum target = gl::GL_TRANSFORM_FEEDBACK);

	void begin(gl::GLenum primitiveMode);
	void pause();
	void resume();
	void end();

    void draw(gl::GLenum primitiveMode) const;

	void setVaryings(const Program * program, gl::GLsizei count, const char ** varyingNames, gl::GLenum bufferMode) const;
    void setVaryings(const Program * program, const std::vector<const char *> & varyingNames, gl::GLenum bufferMode) const;

    template <std::size_t Count>
    void setVaryings(const Program * program, const std::array<const char *, Count> & varyingNames, gl::GLenum bufferMode) const;

    static bool isTransformFeedback(gl::GLuint id);

protected:
    virtual ~TransformFeedback();

	static gl::GLuint genTransformFeedback();
};

} // namespace glow

#include <glow/TransformFeedback.hpp>
