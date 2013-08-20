
#include <glow/VertexAttributeBinding.h>
#include <glow/VertexArrayObject.h>
#include <glow/Version.h>

#include <glow/Error.h>

namespace glow
{

VertexAttributeBinding::VertexAttributeBinding(VertexArrayObject* vao, GLuint bindingIndex)
: _vao(vao)
, _bindingIndex(bindingIndex)
, _vbo(nullptr)
, _attributeIndex(0)
, _implementation(nullptr)
{
#ifndef GL_VERSION_4_3
	_implementation = new VertexAttributeBinding_GL_3_2(this);
#else
	_implementation = Version::current() >= Version(4, 3)
		? (VertexAttributeBindingImplementation*)new VertexAttributeBinding_GL_4_3(this)
		: (VertexAttributeBindingImplementation*)new VertexAttributeBinding_GL_3_2(this);
#endif
	setAttribute(bindingIndex); // as default
}

VertexAttributeBinding::~VertexAttributeBinding()
{
	delete _implementation;
}

void VertexAttributeBinding::setAttribute(GLint attributeIndex)
{
	_attributeIndex = attributeIndex;
	_implementation->bindAttribute(attributeIndex);
}

GLint VertexAttributeBinding::attributeIndex() const
{
	return _attributeIndex;
}

GLuint VertexAttributeBinding::bindingIndex() const
{
	return _bindingIndex;
}

Buffer* VertexAttributeBinding::buffer() const
{
	return _vbo;
}

void VertexAttributeBinding::setBuffer(Buffer* vbo, GLint baseoffset, GLint stride)
{
	_vbo = vbo;
	_implementation->bindBuffer(vbo, baseoffset, stride);
}

void VertexAttributeBinding::setFormat(GLint size, GLenum type, GLboolean normalized, GLuint relativeoffset)
{
	_implementation->setFormat(size, type, normalized, relativeoffset);
}

void VertexAttributeBinding::setIFormat(GLint size, GLenum type, GLuint relativeoffset)
{
	_implementation->setIFormat(size, type, relativeoffset);
}

void VertexAttributeBinding::setLFormat(GLint size, GLuint relativeoffset)
{
	_implementation->setLFormat(size, GL_DOUBLE, relativeoffset);
}



// implementations

VertexAttributeBindingImplementation::VertexAttributeBindingImplementation(VertexAttributeBinding* binding)
: _binding(binding)
{
}

VertexAttributeBindingImplementation::~VertexAttributeBindingImplementation()
{
}

GLint VertexAttributeBindingImplementation::attributeIndex() const
{
	return _binding->_attributeIndex;
}

GLuint VertexAttributeBindingImplementation::bindingIndex() const
{
	return _binding->_bindingIndex;
}

VertexArrayObject* VertexAttributeBindingImplementation::vao() const
{
	return _binding->_vao;
}

Buffer* VertexAttributeBindingImplementation::vbo() const
{
	return _binding->_vbo;
}

// GL 3.2 implementation

VertexAttributeBinding_GL_3_2::VertexAttributeBinding_GL_3_2(VertexAttributeBinding* binding)
: VertexAttributeBindingImplementation(binding)
, _baseoffset(0)
, _stride(0)
, _hasFormat(false)
, _hasBuffer(false)
, _hasAttribute(false)
{
}

void VertexAttributeBinding_GL_3_2::bindAttribute(GLint attributeIndex)
{
	_hasAttribute = true;
	finishIfComplete();
}

void VertexAttributeBinding_GL_3_2::bindBuffer(Buffer* vbo, GLint baseoffset, GLint stride)
{
	_baseoffset = baseoffset;
	_stride = stride;

	_hasBuffer = true;
	finishIfComplete();
}

void VertexAttributeBinding_GL_3_2::setFormat(GLint size, GLenum type, GLboolean normalized, GLuint relativeoffset)
{
	_format = Format(Format::O, size, type, normalized, relativeoffset);

	_hasFormat = true;
	finishIfComplete();
}

void VertexAttributeBinding_GL_3_2::setIFormat(GLint size, GLenum type, GLuint relativeoffset)
{
	_format = Format(Format::I, size, type, GL_FALSE, relativeoffset);

	_hasFormat = true;
	finishIfComplete();
}

void VertexAttributeBinding_GL_3_2::setLFormat(GLint size, GLenum type, GLuint relativeoffset)
{
	_format = Format(Format::L, size, type, GL_FALSE, relativeoffset);

	_hasFormat = true;
	finishIfComplete();
}

void VertexAttributeBinding_GL_3_2::finishIfComplete()
{
	if (_hasAttribute && _hasFormat && _hasBuffer)
	{
		finish();
	}
}

void VertexAttributeBinding_GL_3_2::finish()
{
	vao()->bind();
	vbo()->bind(GL_ARRAY_BUFFER);

	switch (_format.method)
	{
		case Format::I:
			glVertexAttribIPointer(attributeIndex(), _format.size, _format.type, _stride, reinterpret_cast<void*>(_baseoffset+_format.relativeoffset));
			CheckGLError();
			break;
		case Format::L:
			glVertexAttribLPointer(attributeIndex(), _format.size, _format.type, _stride, reinterpret_cast<void*>(_baseoffset+_format.relativeoffset));
			CheckGLError();
			break;
		default:
			glVertexAttribPointer(attributeIndex(), _format.size, _format.type, _format.normalized, _stride, reinterpret_cast<void*>(_baseoffset+_format.relativeoffset));
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
