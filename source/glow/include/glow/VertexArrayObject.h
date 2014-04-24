#pragma once

#include <map>
#include <vector>

#include <glow/glow_api.h>
#include <glow/Object.h>
#include <glow/ref_ptr.h>

namespace glow {
class ObjectVisitor;
class Buffer;
class VertexAttributeBinding;

// http://www.opengl.org/wiki/Vertex_Array_Object
class GLOW_API VertexArrayObject : public Object
{
public:
    VertexArrayObject();
    VertexArrayObject(GLuint id, bool ownsGLObject = true);

    virtual void accept(ObjectVisitor & visitor) override;

    void bind() const;
    static void unbind();

    VertexAttributeBinding* binding(GLuint bindingIndex);
    const VertexAttributeBinding* binding(GLuint bindingIndex) const;

    void enable(GLint attributeIndex);
    void disable(GLint attributeIndex);

    void setAttributeDivisor(GLint attributeIndex, GLuint divisor);

    std::vector<VertexAttributeBinding *> bindings();
    std::vector<const VertexAttributeBinding *> bindings() const;

    // drawing
    void drawArrays(GLenum mode, GLint first, GLsizei count) const;
    void drawArraysInstanced(GLenum mode, GLint first, GLsizei count, GLsizei instanceCount) const;
    void drawArraysInstancedBaseInstance(GLenum mode, GLint first, GLsizei count, GLsizei instanceCount, GLuint baseInstance) const;
    void drawArraysIndirect(GLenum mode, const void * indirect = nullptr) const;

    void multiDrawArrays(GLenum mode, GLint * first, const GLsizei * count, GLsizei drawCount) const;
    void multiDrawArraysIndirect(GLenum mode, const void * indirect, GLsizei drawCount, GLsizei stride) const;

    void drawElements(GLenum mode, GLsizei count, GLenum type, const void * indices = nullptr) const;
    void drawElementsBaseVertex(GLenum mode, GLsizei count, GLenum type, const void * indices, GLint baseVertex) const;
    void drawElementsInstanced(GLenum mode, GLsizei count, GLenum type, const void * indices, GLsizei primitiveCount) const;
    void drawElementsInstancedBaseInstance(GLenum mode, GLsizei count, GLenum type, const void * indices, GLsizei instanceCount, GLuint baseInstance) const;
    void drawElementsInstancedBaseVertex(GLenum mode, GLsizei count, GLenum type, const void * indices, GLsizei instanceCount, GLint baseVertex) const;
    void drawElementsInstancedBaseVertexBaseInstance(GLenum mode, GLsizei count, GLenum type, const void * indices, GLsizei instanceCount, GLint baseVertex, GLuint baseInstance) const;

    void multiDrawElements(GLenum mode, const GLsizei * count, GLenum type, const void ** indices, GLsizei drawCount) const;
    void multiDrawElementsBaseVertex(GLenum mode, const GLsizei * count, GLenum type, const void ** indices, GLsizei drawCount, GLint * baseVertex) const;
    void multiDrawElementsIndirect(GLenum mode, GLenum type, const void * indirect, GLsizei drawCount, GLsizei stride) const;

    void drawRangeElements(GLenum mode, GLuint start, GLuint end, GLsizei count, GLenum type, const void * indices = nullptr) const;
    void drawRangeElementsBaseVertex(GLenum mode, GLuint start, GLuint end, GLsizei count, GLenum type, const void * indices, GLint baseVertex) const;

    // convenience
    struct MultiDrawArraysRange { GLint first; GLsizei count; };
    struct MultiDrawElementsRange { GLsizei count; void * indices; };
    struct MultiDrawElementsBaseVertexRange { GLsizei count; void * indices; GLint baseVertex; };

    void multiDrawArrays(GLenum mode, const std::vector<MultiDrawArraysRange> & ranges) const;
    void multiDrawElements(GLenum mode, GLenum type, const std::vector<MultiDrawElementsRange> & ranges) const;
    void multiDrawElementsBaseVertex(GLenum mode, GLenum type, const std::vector<MultiDrawElementsBaseVertexRange> & ranges) const;
protected:
    virtual ~VertexArrayObject();

    static GLuint genVertexArray();

protected:
    std::map<GLuint, ref_ptr<VertexAttributeBinding >> m_bindings;

};

} // namespace glow
