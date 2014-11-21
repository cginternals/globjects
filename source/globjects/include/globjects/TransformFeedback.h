#pragma once

#include <vector>
#include <array>

#include <glbinding/gl/types.h>

#include <globjects/globjects_api.h>
#include <globjects/Object.h>

namespace globjects
{

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
class GLOBJECTS_API TransformFeedback : public Object
{
public:
	TransformFeedback();

    virtual void accept(ObjectVisitor & visitor) override;

    void bind() const;
    static void unbind();

	void begin(gl::GLenum primitiveMode);
	void pause();
	void resume();
	void end();

    void draw(gl::GLenum primitiveMode) const;

    void setVaryings(const Program * program, gl::GLsizei count, const char ** varyingNames, gl::GLenum bufferMode);

    void setVaryings(const Program * program, const std::vector<std::string> & varyingNames, gl::GLenum bufferMode);

    template <std::size_t Count>
    void setVaryings(const Program * program, const std::array<std::string, Count> & varyingNames, gl::GLenum bufferMode);

    static bool isTransformFeedback(gl::GLuint id);

    virtual gl::GLenum objectType() const override;

protected:
    virtual ~TransformFeedback();

    void bind(gl::GLenum target) const;
    static void unbind(gl::GLenum target);
};

} // namespace globjects

#include <globjects/TransformFeedback.hpp>
