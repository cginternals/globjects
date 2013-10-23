#include <glow/FrameBufferObject.h>

#include <glow/logging.h>
#include <glow/Error.h>
#include <glow/ObjectVisitor.h>
#include <glow/TextureAttachment.h>
#include <glow/RenderBufferAttachment.h>

namespace glow
{

FrameBufferObject FrameBufferObject::s_defaultFBO(0, false);

FrameBufferObject::FrameBufferObject()
: Object(genFrameBuffer())
, m_target(GL_FRAMEBUFFER)
{
}

FrameBufferObject::FrameBufferObject(GLuint id, bool ownsGLObject)
: Object(id, ownsGLObject)
, m_target(GL_FRAMEBUFFER)
{
}

FrameBufferObject* FrameBufferObject::defaultFBO()
{
    return &s_defaultFBO;
}

FrameBufferObject::~FrameBufferObject()
{
	if (ownsGLObject())
	{
		glDeleteFramebuffers(1, &m_id);
		CheckGLError();
	}
}

GLuint FrameBufferObject::genFrameBuffer()
{
	GLuint id = 0;

	glGenFramebuffers(1, &id);
	CheckGLError();

	return id;
}

void FrameBufferObject::accept(ObjectVisitor& visitor)
{
	visitor.visitFrameBufferObject(this);
}

void FrameBufferObject::bind()
{
	glBindFramebuffer(m_target, m_id);
	CheckGLError();
}

void FrameBufferObject::bind(GLenum target)
{
	m_target = target;
	glBindFramebuffer(target, m_id);
	CheckGLError();
}

void FrameBufferObject::unbind()
{
	glBindFramebuffer(m_target, 0);
	CheckGLError();
}

void FrameBufferObject::setParameter(GLenum pname, GLint param)
{
	bind();

	glFramebufferParameteri(m_target, pname, param);
	CheckGLError();
}

void FrameBufferObject::attachTexture(GLenum attachment, Texture* texture, GLint level)
{
	bind();

	glFramebufferTexture(m_target, attachment, texture->id(), level);
	CheckGLError();
	attach(new TextureAttachment(texture, attachment));
}

void FrameBufferObject::attachTexture1D(GLenum attachment, Texture* texture, GLint level)
{
	bind();

	glFramebufferTexture1D(m_target, attachment, texture->target(), texture->id(), level);
	CheckGLError();

	attach(new TextureAttachment(texture, attachment));
}

void FrameBufferObject::attachTexture2D(GLenum attachment, Texture* texture, GLint level)
{
	bind();

	glFramebufferTexture2D(m_target, attachment, texture->target(), texture->id(), level);
	CheckGLError();

	attach(new TextureAttachment(texture, attachment));
}

void FrameBufferObject::attachTextureLayer(GLenum attachment, Texture* texture, GLint level, GLint layer)
{
	bind();

	glFramebufferTextureLayer(m_target, attachment, texture->id(), level, layer);
	CheckGLError();

	attach(new TextureAttachment(texture, attachment));
}

void FrameBufferObject::attachRenderBuffer(GLenum attachment, RenderBufferObject* renderBuffer)
{
	bind();
	renderBuffer->bind();

	glFramebufferRenderbuffer(m_target, attachment, GL_RENDERBUFFER, renderBuffer->id());
	CheckGLError();

	attach(new RenderBufferAttachment(renderBuffer, attachment));
}

void FrameBufferObject::attach(FrameBufferAttachment* attachment)
{
	m_attachments[attachment->attachment()] = attachment;
}

void FrameBufferObject::setReadBuffer(GLenum mode)
{
	bind(GL_READ_FRAMEBUFFER);

	glReadBuffer(mode);
	CheckGLError();
}

void FrameBufferObject::setDrawBuffer(GLenum mode)
{
	bind(GL_DRAW_FRAMEBUFFER);

	glDrawBuffer(mode);
	CheckGLError();
}

void FrameBufferObject::setDrawBuffers(GLsizei n, const GLenum* modes)
{
	bind(GL_DRAW_FRAMEBUFFER);

	glDrawBuffers(n, modes);
	CheckGLError();
}

void FrameBufferObject::setDrawBuffers(const std::vector<GLenum>& modes)
{
	setDrawBuffers(modes.size(), modes.data());
}

void FrameBufferObject::readPixels(GLint x, GLint y, GLsizei width, GLsizei height, GLenum format, GLenum type, GLvoid* data)
{
	bind(GL_FRAMEBUFFER);

	glReadPixels(x, y, width, height, format, type, data);
	CheckGLError();
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
	CheckGLError();
}

GLenum FrameBufferObject::checkStatus()
{
	bind();

	GLenum result = glCheckFramebufferStatus(m_target);
	CheckGLError();
	return result;
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

FrameBufferAttachment* FrameBufferObject::attachment(GLenum attachment)
{
	return m_attachments[attachment];
}

std::vector<FrameBufferAttachment*> FrameBufferObject::attachments()
{
	std::vector<FrameBufferAttachment*> attachments;

	for (std::pair<GLenum, ref_ptr<FrameBufferAttachment>> pair: m_attachments)
	{
		attachments.push_back(pair.second);
	}

	return attachments;
}

} // namespace glow
