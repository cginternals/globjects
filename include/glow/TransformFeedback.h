#pragma once

#include <glow/glow.h>

#include <glow/Object.h>
#include <glow/Program.h>
#include <glow/Array.h>

namespace glow
{

class GLOW_API TransformFeedback : public Object
{
public:
	TransformFeedback();
	~TransformFeedback();
	
	void bind();
	void unbind();
	
	void begin(GLenum primitiveMode);
	void pause();
	void resume();
	void end();
	
	void draw(GLenum primitiveMode);
	
	void setVaryings(Program* program, GLsizei count, const char** varyingNames, GLenum bufferMode);
	void setVaryings(Program* program, const Array<std::string>& varyingNames, GLenum bufferMode);
protected:
	static GLuint genTransformFeedback();
};

} // namespace glow
