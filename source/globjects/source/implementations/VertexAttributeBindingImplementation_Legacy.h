#pragma once

#include <glbinding/gl/types.h>

#include <globjects/base/Singleton.h>

#include <globjects/globjects_api.h>

#include "AbstractVertexAttributeBindingImplementation.h"

struct BindingData;

namespace glo {

class VertexAttributeBindingImplementation_Legacy : public AbstractVertexAttributeBindingImplementation
        , public Singleton<VertexAttributeBindingImplementation_Legacy>
{
public:
    VertexAttributeBindingImplementation_Legacy();
    virtual ~VertexAttributeBindingImplementation_Legacy();

    virtual void enable(const VertexArray * vertexArray, gl::GLint attributeIndex) const override;
    virtual void disable(const VertexArray * vertexArray, gl::GLint attributeIndex) const override;

    virtual void setAttributeDivisor(const VertexAttributeBinding * binding, gl::GLuint divisor) const override;

    virtual void bindAttribute(const VertexAttributeBinding * binding, gl::GLint attributeIndex) const override;
    virtual void bindBuffer(const VertexAttributeBinding * binding, const Buffer * vbo, gl::GLint baseoffset, gl::GLint stride) const override;

    virtual void setFormat(const VertexAttributeBinding * binding, gl::GLint size, gl::GLenum type, gl::GLboolean normalized, gl::GLuint relativeoffset) const override;
    virtual void setIFormat(const VertexAttributeBinding * binding, gl::GLint size, gl::GLenum type, gl::GLuint relativeoffset) const override;
    virtual void setLFormat(const VertexAttributeBinding * binding, gl::GLint size, gl::GLenum type, gl::GLuint relativeoffset) const override;
protected:
    BindingData * & bindingData(const VertexAttributeBinding * binding) const;

    void finishIfComplete(const VertexAttributeBinding * binding) const;
    void finish(const VertexAttributeBinding * binding) const;
};

} // namespace glo
