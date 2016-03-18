#include <globjects/ObjectVisitor.h>

#include <cassert>

#include <globjects/Object.h>

namespace globjects
{

ObjectVisitor::ObjectVisitor()
{
}

ObjectVisitor::~ObjectVisitor()
{
}

void ObjectVisitor::visit(Object* object)
{
    assert(object != nullptr);

    object->accept(*this);
}

void ObjectVisitor::visitBuffer(Buffer* /*buffer*/)
{
}

void ObjectVisitor::visitFrameBufferObject(Framebuffer* /*fbo*/)
{
}

void ObjectVisitor::visitProgram(Program* /*program*/)
{
}

void ObjectVisitor::visitProgramPipeline(ProgramPipeline* /*programPipeline*/)
{
}

void ObjectVisitor::visitQuery(Query* /*query*/)
{
}

void ObjectVisitor::visitRenderBufferObject(Renderbuffer* /*rbo*/)
{
}

void ObjectVisitor::visitSampler(Sampler * /*sampler*/)
{
}

void ObjectVisitor::visitShader(Shader* /*shader*/)
{
}

void ObjectVisitor::visitTexture(Texture* /*texture*/)
{
}

void ObjectVisitor::visitTransformFeedback(TransformFeedback* /*transformfeedback*/)
{
}

void ObjectVisitor::visitVertexArray(VertexArray* /*vao*/)
{
}

} // namespace globjects
