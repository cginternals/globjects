
#include <glow/VertexArrayObject.h>
#include <glow/Version.h>
#include <glow/Error.h>
#include <glow/Buffer.h>

#include <glow/VertexAttributeBinding.h>

namespace glow
{

VertexAttributeBinding::VertexAttributeBinding(
    VertexArrayObject * vao
,   const GLuint bindingIndex)
: m_vao(vao)
, m_bindingIndex(bindingIndex)
, m_attributeIndex(0)
, m_vbo(nullptr)
, m_implementation(nullptr)
{
    m_implementation = Version::current() >= Version(4, 3)
		? (VertexAttributeBindingImplementation*)new VertexAttributeBinding_GL_4_3(this)
		: (VertexAttributeBindingImplementation*)new VertexAttributeBinding_GL_3_2(this);

	setAttribute(bindingIndex); // as default
}

VertexAttributeBinding::~VertexAttributeBinding()
{
    delete m_implementation;
}

void VertexAttributeBinding::setAttribute(GLint attributeIndex)
{
    m_attributeIndex = attributeIndex;
    m_implementation->bindAttribute(attributeIndex);
}

GLuint VertexAttributeBinding::attributeIndex() const
{
    return m_attributeIndex;
}

GLuint VertexAttributeBinding::bindingIndex() const
{
    return m_bindingIndex;
}

Buffer* VertexAttributeBinding::buffer() const
{
    return m_vbo;
}

void VertexAttributeBinding::setBuffer(Buffer* vbo, GLint baseoffset, GLint stride)
{
    m_vbo = vbo;
    m_implementation->bindBuffer(vbo, baseoffset, stride);
}

void VertexAttributeBinding::setFormat(GLint size, GLenum type, GLboolean normalized, GLuint relativeoffset)
{
    m_implementation->setFormat(size, type, normalized, relativeoffset);
}

void VertexAttributeBinding::setIFormat(GLint size, GLenum type, GLuint relativeoffset)
{
    m_implementation->setIFormat(size, type, relativeoffset);
}

void VertexAttributeBinding::setLFormat(GLint size, GLuint relativeoffset)
{
    m_implementation->setLFormat(size, GL_DOUBLE, relativeoffset);
}



// implementations

VertexAttributeBindingImplementation::VertexAttributeBindingImplementation(VertexAttributeBinding* binding)
: m_binding(binding)
{
}

VertexAttributeBindingImplementation::~VertexAttributeBindingImplementation()
{
}

GLuint VertexAttributeBindingImplementation::attributeIndex() const
{
    return m_binding->m_attributeIndex;
}

GLuint VertexAttributeBindingImplementation::bindingIndex() const
{
    return m_binding->m_bindingIndex;
}

VertexArrayObject* VertexAttributeBindingImplementation::vao() const
{
    return m_binding->m_vao;
}

Buffer* VertexAttributeBindingImplementation::vbo() const
{
    return m_binding->m_vbo;
}

// GL 3.2 implementation

VertexAttributeBinding_GL_3_2::VertexAttributeBinding_GL_3_2(VertexAttributeBinding* binding)
: VertexAttributeBindingImplementation(binding)
, m_baseoffset(0)
, m_stride(0)
, m_hasFormat(false)
, m_hasBuffer(false)
, m_hasAttribute(false)
{
}

void VertexAttributeBinding_GL_3_2::bindAttribute(GLint attributeIndex)
{
    m_hasAttribute = true;
	finishIfComplete();
}

void VertexAttributeBinding_GL_3_2::bindBuffer(Buffer* vbo, GLint baseoffset, GLint stride)
{
    m_baseoffset = baseoffset;
    m_stride = stride;

    m_hasBuffer = true;
	finishIfComplete();
}

void VertexAttributeBinding_GL_3_2::setFormat(GLint size, GLenum type, GLboolean normalized, GLuint relativeoffset)
{
    m_format = Format(Format::O, size, type, normalized, relativeoffset);

    m_hasFormat = true;
	finishIfComplete();
}

void VertexAttributeBinding_GL_3_2::setIFormat(GLint size, GLenum type, GLuint relativeoffset)
{
    m_format = Format(Format::I, size, type, GL_FALSE, relativeoffset);

    m_hasFormat = true;
	finishIfComplete();
}

void VertexAttributeBinding_GL_3_2::setLFormat(GLint size, GLenum type, GLuint relativeoffset)
{
    m_format = Format(Format::L, size, type, GL_FALSE, relativeoffset);

    m_hasFormat = true;
	finishIfComplete();
}

void VertexAttributeBinding_GL_3_2::finishIfComplete()
{
    if (m_hasAttribute && m_hasFormat && m_hasBuffer)
	{
		finish();
	}
}

void VertexAttributeBinding_GL_3_2::finish()
{
	vao()->bind();
	vbo()->bind(GL_ARRAY_BUFFER);

    const GLuint attribute = attributeIndex();

    switch (m_format.method)
	{
	case Format::I:
        glVertexAttribIPointer(attribute, m_format.size, m_format.type, m_stride, reinterpret_cast<void*>(m_baseoffset + m_format.relativeoffset));
		CheckGLError();
		break;
	case Format::L:
        glVertexAttribLPointer(attribute, m_format.size, m_format.type, m_stride, reinterpret_cast<void*>(m_baseoffset + m_format.relativeoffset));
		CheckGLError();
		break;
	default:
        glVertexAttribPointer(attribute, m_format.size, m_format.type, m_format.normalized, m_stride, reinterpret_cast<void*>(m_baseoffset + m_format.relativeoffset));
		CheckGLError();
	}
}

VertexAttributeBinding_GL_3_2::Format::Format()
: method(O)
, size(0)
, type(0)
, normalized(GL_FALSE)
, relativeoffset(0)
{
}

VertexAttributeBinding_GL_3_2::Format::Format(Method method, GLint size, GLenum type, GLboolean normalized, GLuint relativeoffset)
: method(method)
, size(size)
, type(type)
, normalized(normalized)
, relativeoffset(relativeoffset)
{
}

// GL 4.3 implementation

#ifdef GL_VERSION_4_3

VertexAttributeBinding_GL_4_3::VertexAttributeBinding_GL_4_3(VertexAttributeBinding* binding)
: VertexAttributeBindingImplementation(binding)
{
}

void VertexAttributeBinding_GL_4_3::bindAttribute(GLint attributeIndex)
{
	vao()->bind();

	glVertexAttribBinding(attributeIndex, bindingIndex());
	CheckGLError();
}

void VertexAttributeBinding_GL_4_3::bindBuffer(Buffer* vbo, GLint baseoffset, GLint stride)
{
	vao()->bind();

	glBindVertexBuffer(bindingIndex(), vbo->id(), baseoffset, stride);
	CheckGLError();
}

void VertexAttributeBinding_GL_4_3::setFormat(GLint size, GLenum type, GLboolean normalized, GLuint relativeoffset)
{
	vao()->bind();

	glVertexAttribFormat(attributeIndex(), size, type, normalized, relativeoffset);
	CheckGLError();
}

void VertexAttributeBinding_GL_4_3::setIFormat(GLint size, GLenum type, GLuint relativeoffset)
{
	vao()->bind();

	glVertexAttribIFormat(attributeIndex(), size, type, relativeoffset);
	CheckGLError();
}

void VertexAttributeBinding_GL_4_3::setLFormat(GLint size, GLenum type, GLuint relativeoffset)
{
	vao()->bind();

	glVertexAttribLFormat(attributeIndex(), size, type, relativeoffset);
	CheckGLError();
}

#endif

} // namespace glow
