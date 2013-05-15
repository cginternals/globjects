#pragma once

#include <glow/Object.h>

// http://www.opengl.org/wiki/Framebuffer_Object

namespace glow {

class FrameBufferObject : public Object
{
public:
	FrameBufferObject();
	~FrameBufferObject();

	void bind(GLenum target = GL_FRAMEBUFFER);
	void unbind();
protected:
	GLenum _target;
};

/*

to implement:

 void glFramebufferTexture1D​(GLenum target​, GLenum attachment​, GLenum textarget​, GLuint texture​, GLint level​);
 void glFramebufferTexture2D​(GLenum target​, GLenum attachment​, GLenum textarget​, GLuint texture​, GLint level​);
 void glFramebufferTextureLayer​(GLenum target​, GLenum attachment​, GLuint texture​, GLint level​, GLint layer​);

void glFramebufferRenderbuffer​(GLenum target​, GLenum attachment​, GLenum renderbuffertarget​, GLuint renderbuffer​);

void glFramebufferTexture​(GLenum target​, GLenum attachment​, GLuint texture​, GLint level​);

void glFramebufferParameteri​(GLenum target​, GLenum pname​, GLint param​);

GLenum glCheckFramebufferStatus​(GLenum target​);

*/

} // namespace glow
