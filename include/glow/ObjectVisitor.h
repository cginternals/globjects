#pragma once

#include <glow/glow.h>

#include <glow/Object.h>
#include <glow/Buffer.h>
#include <glow/FrameBufferObject.h>
#include <glow/Program.h>
#include <glow/Query.h>
#include <glow/RenderBufferObject.h>
#include <glow/Shader.h>
#include <glow/Texture.h>
#include <glow/TransformFeedback.h>
#include <glow/VertexArrayObject.h>

namespace glow
{

/** \brief Implements a Visitor Pattern to iterate over all tracked glow objects.
 
    Subclasses should implement the appropriate visit*() methods for those types
    they want to handle.
 */
class GLOW_API ObjectVisitor
{
public:
	ObjectVisitor();
	virtual ~ObjectVisitor();

	virtual void visit(Object* object);

	virtual void visitBuffer(Buffer* buffer);
	virtual void visitFrameBufferObject(FrameBufferObject* fbo);
	virtual void visitProgram(Program* program);
	virtual void visitQuery(Query* query);
	virtual void visitRenderBufferObject(RenderBufferObject* rbo);
	virtual void visitShader(Shader* shader);
	virtual void visitTexture(Texture* texture);
	virtual void visitTransformFeedback(TransformFeedback* transformfeedback);
	virtual void visitVertexArrayObject(VertexArrayObject* vao);
};

} // namespace glow
