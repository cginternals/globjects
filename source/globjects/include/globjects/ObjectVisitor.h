#pragma once

#include <globjects/globjects_api.h>

namespace globjects
{
class Object;
class Buffer;
class Framebuffer;
class Program;
class ProgramPipeline;
class Query;
class Renderbuffer;
class Sampler;
class Shader;
class Texture;
class TransformFeedback;
class VertexArray;


/** \brief Implements a Visitor Pattern to iterate over all tracked globjects objects.
 
    Subclasses should implement the appropriate visit*() methods for those types
    they want to handle.
 */
class GLOBJECTS_API ObjectVisitor
{
public:
    ObjectVisitor();
    virtual ~ObjectVisitor();

    virtual void visit(Object * object);

    virtual void visitBuffer(Buffer * buffer);
    virtual void visitFrameBufferObject(Framebuffer * fbo);
    virtual void visitProgram(Program * program);
    virtual void visitProgramPipeline(ProgramPipeline * programPipeline);
    virtual void visitQuery(Query * query);
    virtual void visitRenderBufferObject(Renderbuffer * rbo);
    virtual void visitSampler(Sampler * sampler);
    virtual void visitShader(Shader * shader);
    virtual void visitTexture(Texture * texture);
    virtual void visitTransformFeedback(TransformFeedback * transformfeedback);
    virtual void visitVertexArray(VertexArray * vao);
};

} // namespace globjects
