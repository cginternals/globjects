#pragma once

#include <glow/glow_api.h>

namespace glow
{
class Object;
class Buffer;
class FrameBufferObject;
class Program;
class Query;
class RenderBufferObject;
class Sampler;
class Shader;
class Texture;
class TransformFeedback;
class VertexArrayObject;


/** \brief Implements a Visitor Pattern to iterate over all tracked glow objects.
 
    Subclasses should implement the appropriate visit*() methods for those types
    they want to handle.
 */
class GLOW_API ObjectVisitor
{
public:
	ObjectVisitor();
	virtual ~ObjectVisitor();

	virtual void visit(Object * object);

	virtual void visitBuffer(Buffer * buffer);
	virtual void visitFrameBufferObject(FrameBufferObject * fbo);
	virtual void visitProgram(Program * program);
	virtual void visitQuery(Query * query);
	virtual void visitRenderBufferObject(RenderBufferObject * rbo);
    virtual void visitSampler(Sampler * sampler);
	virtual void visitShader(Shader * shader);
	virtual void visitTexture(Texture * texture);
	virtual void visitTransformFeedback(TransformFeedback * transformfeedback);
	virtual void visitVertexArrayObject(VertexArrayObject * vao);
};

} // namespace glow
