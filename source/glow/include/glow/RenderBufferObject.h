#pragma once

#include <GL/glew.h>

#include <glow/glow_api.h>
#include <glow/Object.h>

namespace glow 
{

class ObjectVisitor;

/** \brief Encapsulates OpenGL render buffer objects.
 
    \see http://www.opengl.org/wiki/Renderbuffer_Objects
 */
class GLOW_API RenderBufferObject : public Object
{
public:
	RenderBufferObject();
	virtual ~RenderBufferObject();

    virtual void accept(ObjectVisitor & visitor) override;

    void bind() const;
    void unbind() const;

	void storage(
        GLenum internalformat
    ,   GLsizei width
    ,   GLsizei height);
	void storageMultisample(
        GLsizei samples
    ,   GLenum internalformat
    ,   GLsizei width
    ,   GLsizei height);

    GLint getParameter(GLenum pname) const;

protected:
	static GLuint genRenderBuffer();
};

} // namespace glow
