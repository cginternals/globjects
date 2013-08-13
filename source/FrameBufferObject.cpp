
#include <sstream>

#include <glow/FrameBufferObject.h>
#include <glow/logging.h>

namespace glow
{

ColorAttachment::ColorAttachment(GLenum attachment)
: _attachment(attachment)
{
}

GLenum ColorAttachment::attachment() const
{
	return _attachment;
}

bool ColorAttachment::isTextureAttachment() const
{
	return false;
}

bool ColorAttachment::isRenderBufferAttachment() const
{
	return false;
}

TextureAttachment::TextureAttachment(Texture* texture, GLenum attachment)
: ColorAttachment(attachment)
, _texture(texture)
{
}

bool TextureAttachment::isTextureAttachment() const
{
	return true;
}

Texture* TextureAttachment::texture()
{
	return _texture;
}

RenderBufferAttachment::RenderBufferAttachment(RenderBufferObject* renderBuffer, GLenum attachment)
: ColorAttachment(attachment)
, _renderBuffer(renderBuffer)
{
}

bool RenderBufferAttachment::isRenderBufferAttachment() const
{
	return true;
}

RenderBufferObject* RenderBufferAttachment::renderBuffer()
{
	return _renderBuffer;
}


FrameBufferObject FrameBufferObject::_defaultFBO(0, false);

FrameBufferObject::FrameBufferObject()
: Object(genFrameBuffer())
, _target(GL_FRAMEBUFFER)
{
}

FrameBufferObject::FrameBufferObject(GLuint id, bool ownsGLObject)
: Object(id, ownsGLObject)
, _target(GL_FRAMEBUFFER)
{
}

FrameBufferObject* FrameBufferObject::defaultFBO()
{
    return &_defaultFBO;
}

FrameBufferObject::~FrameBufferObject()
{
	if (ownsGLObject())
    {
        glDeleteFramebuffers(1, &m_id);
    }
}

GLuint FrameBufferObject::genFrameBuffer()
{
	GLuint id = 0;
	glGenFramebuffers(1, &id);
	return id;
}

void FrameBufferObject::bind()
{
	glBindFramebuffer(_target, m_id);
}

void FrameBufferObject::bind(GLenum target)
{
	_target = target;
	glBindFramebuffer(target, m_id);
}

void FrameBufferObject::unbind()
{
	glBindFramebuffer(_target, 0);
}

void FrameBufferObject::setParameter(GLenum pname, GLint param)
{
	bind();
	glFramebufferParameteri(_target, pname, param);
}

void FrameBufferObject::attachTexture(GLenum attachment, Texture* texture, GLint level)
{
	bind();
	glFramebufferTexture(_target, attachment, texture->id(), level);
	attach(new TextureAttachment(texture, attachment));
}

void FrameBufferObject::attachTexture1D(GLenum attachment, Texture* texture, GLint level)
{
	bind();
	glFramebufferTexture1D(_target, attachment, texture->target(), texture->id(), level);
	attach(new TextureAttachment(texture, attachment));
}

void FrameBufferObject::attachTexture2D(GLenum attachment, Texture* texture, GLint level)
{
	bind();
	glFramebufferTexture2D(_target, attachment, texture->target(), texture->id(), level);
	attach(new TextureAttachment(texture, attachment));
}

void FrameBufferObject::attachTextureLayer(GLenum attachment, Texture* texture, GLint level, GLint layer)
{
	bind();
	glFramebufferTextureLayer(_target, attachment, texture->id(), level, layer);
	attach(new TextureAttachment(texture, attachment));
}

void FrameBufferObject::attachRenderBuffer(GLenum attachment, RenderBufferObject* renderBuffer)
{
	bind();
	renderBuffer->bind();
	glFramebufferRenderbuffer(_target, attachment, GL_RENDERBUFFER, renderBuffer->id());
	attach(new RenderBufferAttachment(renderBuffer, attachment));
}

void FrameBufferObject::attach(ColorAttachment* attachment)
{
	_attachments[attachment->attachment()] = attachment;
}

void FrameBufferObject::setReadBuffer(GLenum mode)
{
    bind(GL_READ_FRAMEBUFFER);
    glReadBuffer(mode);
}

void FrameBufferObject::setDrawBuffer(GLenum mode)
{
    bind(GL_DRAW_FRAMEBUFFER);
    glDrawBuffer(mode);
}

void FrameBufferObject::setDrawBuffers(GLsizei n, const GLenum* modes)
{
    bind(GL_DRAW_FRAMEBUFFER);
    glDrawBuffers(n, modes);
}

void FrameBufferObject::readPixels(GLint x, GLint y, GLsizei width, GLsizei height, GLenum format, GLenum type, GLvoid* data)
{
	bind(GL_FRAMEBUFFER);
    glReadPixels(x, y, width, height, format, type, data);
}

void FrameBufferObject::readPixelsToBuffer(GLint x, GLint y, GLsizei width, GLsizei height, GLenum format, GLenum type, Buffer* pbo)
{
	pbo->bind(GL_PIXEL_PACK_BUFFER);
	readPixels(x, y, width, height, format, type, 0);
	pbo->unbind();
}

void FrameBufferObject::blit(GLint srcX0, GLint srcY0, GLint srcX1, GLint srcY1, GLint destX0, GLint destY0, GLint destX1, GLint destY1, GLbitfield mask, GLenum filter)
{
	glBlitFramebuffer(srcX0, srcY0, srcX1, srcY1, destX0, destY0, destX1, destY1, mask, filter);
}

GLenum FrameBufferObject::checkStatus()
{
	bind();
	return glCheckFramebufferStatus(_target);
}

std::string FrameBufferObject::statusString()
{
	return statusString(checkStatus());
}

std::string FrameBufferObject::statusString(GLenum status)
{
	switch (status)
	{
		case GL_FRAMEBUFFER_COMPLETE:
			return "GL_FRAMEBUFFER_COMPLETE​";
		case GL_FRAMEBUFFER_UNDEFINED:
			return "GL_FRAMEBUFFER_UNDEFINED";
		case GL_FRAMEBUFFER_INCOMPLETE_ATTACHMENT:
			return "GL_FRAMEBUFFER_INCOMPLETE_ATTACHMENT";
		case GL_FRAMEBUFFER_INCOMPLETE_MISSING_ATTACHMENT:
			return "GL_FRAMEBUFFER_INCOMPLETE_MISSING_ATTACHMENT";
		case GL_FRAMEBUFFER_INCOMPLETE_DRAW_BUFFER:
			return "GL_FRAMEBUFFER_INCOMPLETE_DRAW_BUFFER";
		case GL_FRAMEBUFFER_INCOMPLETE_READ_BUFFER:
			return "GL_FRAMEBUFFER_INCOMPLETE_READ_BUFFER";
		case GL_FRAMEBUFFER_UNSUPPORTED:
			return "GL_FRAMEBUFFER_UNSUPPORTED";
		case GL_FRAMEBUFFER_INCOMPLETE_MULTISAMPLE:
			return "GL_FRAMEBUFFER_INCOMPLETE_MULTISAMPLE";
		case GL_FRAMEBUFFER_INCOMPLETE_LAYER_TARGETS:
			return "GL_FRAMEBUFFER_INCOMPLETE_LAYER_TARGETS";
		default:
			return "unknown framebuffer error";
	}
}

void FrameBufferObject::printStatus(bool onlyErrors)
{
	GLenum status = checkStatus();

	if (onlyErrors && status == GL_FRAMEBUFFER_COMPLETE) return;

	if (status == GL_FRAMEBUFFER_COMPLETE)
	{
		info() << statusString(GL_FRAMEBUFFER_COMPLETE);
	}
	else
	{
		std::stringstream ss;
		ss.flags(std::ios::hex | std::ios::showbase);
		ss << status;

		critical() << statusString(status) << " (" << ss.str() << ")";
	}
	}

ColorAttachment* FrameBufferObject::attachment(GLenum attachment)
{
	return _attachments[attachment];
}

} // namespace glow
