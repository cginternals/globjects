#include <cassert>

#include <glow/Object.h>

#include <glow/ObjectVisitor.h>

namespace glow
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

void ObjectVisitor::visitBuffer(Buffer* buffer)
{
    assert(buffer != nullptr);
}

void ObjectVisitor::visitFrameBufferObject(FrameBufferObject* fbo)
{
    assert(fbo != nullptr);
}

void ObjectVisitor::visitProgram(Program* program)
{
    assert(program != nullptr);
}

void ObjectVisitor::visitQuery(Query* query)
{
    assert(query != nullptr);
}

void ObjectVisitor::visitRenderBufferObject(RenderBufferObject* rbo)
{
    assert(rbo != nullptr);
}

void ObjectVisitor::visitShader(Shader* shader)
{
    assert(shader != nullptr);
}

void ObjectVisitor::visitTexture(Texture* texture)
{
    assert(texture != nullptr);
}

void ObjectVisitor::visitTransformFeedback(TransformFeedback* transformfeedback)
{
    assert(transformfeedback != nullptr);
}

void ObjectVisitor::visitVertexArrayObject(VertexArrayObject* vao)
{
    assert(vao != nullptr);
}

} // namespace glow
