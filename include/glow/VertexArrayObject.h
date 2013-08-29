#pragma once

#include <map>
#include <set>
#include <vector>

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

	virtual void accept(ObjectVisitor& visitor);

	void bind();
	void unbind();

	VertexAttributeBinding* binding(GLuint bindingIndex);

	void enable(GLint attributeIndex);
	void disable(GLint attributeIndex);

	std::vector<VertexAttributeBinding*> bindings();
protected:
	GLuint m_nextBindingIndex;
	std::map<GLuint, ref_ptr<VertexAttributeBinding>> m_bindings;

	static GLuint genVertexArray();
};

} // namespace glow
