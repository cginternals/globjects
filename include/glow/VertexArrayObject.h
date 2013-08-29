#pragma once

#include <map>
#include <set>

#include <glow/glow.h>

#include <glow/Object.h>
#include <glow/Buffer.h>
#include <glow/Buffer.h>
#include <glow/VertexAttributeBinding.h>
#include <glow/ref_ptr.hpp>

// http://www.opengl.org/wiki/Vertex_Array_Object

namespace glow {

class GLOW_API VertexArrayObject : public Object
{
public:
	VertexArrayObject();
	VertexArrayObject(GLuint id, bool ownsGLObject = true);
	virtual ~VertexArrayObject();

	virtual const char* typeName() const;
	virtual void accept(ObjectVisitor& visitor);

	void bind();
	void unbind();

	VertexAttributeBinding* binding(GLuint bindingIndex);

	void enable(GLint attributeIndex);
	void disable(GLint attributeIndex);
protected:
	GLuint _nextBindingIndex;
	std::map<GLuint, ref_ptr<VertexAttributeBinding>> _bindings;

	static GLuint genVertexArray();
};

} // namespace glow
