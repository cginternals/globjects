#include <cassert>

#include <globjects/Object.h>

#include <globjects/ObjectVisitor.h>

namespace glo
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

void ObjectVisitor::visitFrameBufferObject(FrameBufferObject* /*fbo*/)
{
}

void ObjectVisitor::visitProgram(Program* /*program*/)
{
}

void ObjectVisitor::visitQuery(Query* /*query*/)
{
}

void ObjectVisitor::visitRenderBufferObject(RenderBufferObject* /*rbo*/)
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

void ObjectVisitor::visitVertexArrayObject(VertexArrayObject* /*vao*/)
{
}

} // namespace glo
