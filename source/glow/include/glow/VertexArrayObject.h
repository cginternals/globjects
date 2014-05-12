#pragma once

#include <map>
#include <vector>

#include <glbinding/types.h>

#include <glowbase/ref_ptr.h>

#include <glow/glow_api.h>
#include <glow/Object.h>

namespace glow {
class ObjectVisitor;
class Buffer;
class VertexAttributeBinding;

// http://www.opengl.org/wiki/Vertex_Array_Object
class GLOW_API VertexArrayObject : public Object
{
public:
    VertexArrayObject();
    static VertexArrayObject * fromId(gl::GLuint id, bool takeOwnership = false);

    virtual void accept(ObjectVisitor & visitor) override;

    void bind() const;
    static void unbind();

    VertexAttributeBinding* binding(gl::GLuint bindingIndex);
    const VertexAttributeBinding* binding(gl::GLuint bindingIndex) const;

    void enable(gl::GLint attributeIndex);
    void disable(gl::GLint attributeIndex);

    void setAttributeDivisor(gl::GLint attributeIndex, gl::GLuint divisor);

    std::vector<VertexAttributeBinding *> bindings();
    std::vector<const VertexAttributeBinding *> bindings() const;

    // drawing
    void drawArrays(gl::GLenum mode, gl::GLint first, gl::GLsizei count) const;
    void drawArraysInstanced(gl::GLenum mode, gl::GLint first, gl::GLsizei count, gl::GLsizei instanceCount) const;
    void drawArraysInstancedBaseInstance(gl::GLenum mode, gl::GLint first, gl::GLsizei count, gl::GLsizei instanceCount, gl::GLuint baseInstance) const;
    void drawArraysIndirect(gl::GLenum mode, const void * indirect = nullptr) const;

    void multiDrawArrays(gl::GLenum mode, gl::GLint * first, const gl::GLsizei * count, gl::GLsizei drawCount) const;
    void multiDrawArraysIndirect(gl::GLenum mode, const void * indirect, gl::GLsizei drawCount, gl::GLsizei stride) const;

    void drawElements(gl::GLenum mode, gl::GLsizei count, gl::GLenum type, const void * indices = nullptr) const;
    void drawElementsBaseVertex(gl::GLenum mode, gl::GLsizei count, gl::GLenum type, const void * indices, gl::GLint baseVertex) const;
    void drawElementsInstanced(gl::GLenum mode, gl::GLsizei count, gl::GLenum type, const void * indices, gl::GLsizei primitiveCount) const;
    void drawElementsInstancedBaseInstance(gl::GLenum mode, gl::GLsizei count, gl::GLenum type, const void * indices, gl::GLsizei instanceCount, gl::GLuint baseInstance) const;
    void drawElementsInstancedBaseVertex(gl::GLenum mode, gl::GLsizei count, gl::GLenum type, const void * indices, gl::GLsizei instanceCount, gl::GLint baseVertex) const;
    void drawElementsInstancedBaseVertexBaseInstance(gl::GLenum mode, gl::GLsizei count, gl::GLenum type, const void * indices, gl::GLsizei instanceCount, gl::GLint baseVertex, gl::GLuint baseInstance) const;

    void multiDrawElements(gl::GLenum mode, const gl::GLsizei * count, gl::GLenum type, const void ** indices, gl::GLsizei drawCount) const;
    void multiDrawElementsBaseVertex(gl::GLenum mode, const gl::GLsizei * count, gl::GLenum type, const void ** indices, gl::GLsizei drawCount, gl::GLint * baseVertex) const;
    void multiDrawElementsIndirect(gl::GLenum mode, gl::GLenum type, const void * indirect, gl::GLsizei drawCount, gl::GLsizei stride) const;

    void drawRangeElements(gl::GLenum mode, gl::GLuint start, gl::GLuint end, gl::GLsizei count, gl::GLenum type, const void * indices = nullptr) const;
    void drawRangeElementsBaseVertex(gl::GLenum mode, gl::GLuint start, gl::GLuint end, gl::GLsizei count, gl::GLenum type, const void * indices, gl::GLint baseVertex) const;

    // convenience
    struct MultiDrawArraysRange { gl::GLint first; gl::GLsizei count; };
    struct MultiDrawElementsRange { gl::GLsizei count; void * indices; };
    struct MultiDrawElementsBaseVertexRange { gl::GLsizei count; void * indices; gl::GLint baseVertex; };

    void multiDrawArrays(gl::GLenum mode, const std::vector<MultiDrawArraysRange> & ranges) const;
    void multiDrawElements(gl::GLenum mode, gl::GLenum type, const std::vector<MultiDrawElementsRange> & ranges) const;
    void multiDrawElementsBaseVertex(gl::GLenum mode, gl::GLenum type, const std::vector<MultiDrawElementsBaseVertexRange> & ranges) const;
protected:
    VertexArrayObject(gl::GLuint id, bool takeOwnership);
    virtual ~VertexArrayObject();

    static gl::GLuint genVertexArray();

protected:
    std::map<gl::GLuint, ref_ptr<VertexAttributeBinding >> m_bindings;

};

} // namespace glow
