#include <glow/FrameBufferObject.h>

#include <cassert>

#include <glow/logging.h>
#include <glow/Error.h>
#include <glow/ObjectVisitor.h>
#include <glow/TextureAttachment.h>
#include <glow/FrameBufferAttachment.h>
#include <glow/RenderBufferAttachment.h>
#include <glow/Buffer.h>
#include <glow/RenderBufferObject.h>
#include <glow/Texture.h>
#include "pixelformat.h"

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

FrameBufferObject * FrameBufferObject::defaultFBO()
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

void FrameBufferObject::bind() const
{
	glBindFramebuffer(m_target, m_id);
	CheckGLError();
}

void FrameBufferObject::bind(GLenum target) const
{
	m_target = target;
	glBindFramebuffer(target, m_id);
	CheckGLError();
}

void FrameBufferObject::unbind() const
{
	glBindFramebuffer(m_target, 0);
	CheckGLError();
}

void FrameBufferObject::unbind(GLenum target)
{
    glBindFramebuffer(target, 0);
    CheckGLError();
}

void FrameBufferObject::setParameter(GLenum pname, GLint param)
{
	bind();

	glFramebufferParameteri(m_target, pname, param);
	CheckGLError();
}

int FrameBufferObject::getAttachmentParameter(GLenum attachment, GLenum pname) const
{
    bind();

    int result = 0;

    glGetFramebufferAttachmentParameteriv(m_target, attachment, pname, &result);
    CheckGLError();

    return result;
}

void FrameBufferObject::attachTexture(GLenum attachment, Texture* texture, GLint level)
{
    assert(texture != nullptr);

	bind();

	glFramebufferTexture(m_target, attachment, texture->id(), level);
	CheckGLError();

    attach(new TextureAttachment(texture, attachment, level));
}

void FrameBufferObject::attachTexture1D(GLenum attachment, Texture* texture, GLint level)
{
    assert(texture != nullptr);

	bind();

	glFramebufferTexture1D(m_target, attachment, texture->target(), texture->id(), level);
	CheckGLError();

    attach(new TextureAttachment(texture, attachment, level));
}

void FrameBufferObject::attachTexture2D(GLenum attachment, Texture* texture, GLint level)
{
    assert(texture != nullptr);

	bind();

	glFramebufferTexture2D(m_target, attachment, texture->target(), texture->id(), level);
	CheckGLError();

    attach(new TextureAttachment(texture, attachment, level));
}

void FrameBufferObject::attachTexture3D(GLenum attachment, Texture * texture, GLint level, GLint layer)
{
    assert(texture != nullptr);

    bind();

    glFramebufferTexture3D(m_target, attachment, texture->target(), texture->id(), level, layer);
    CheckGLError();

    attach(new TextureAttachment(texture, attachment, level));
}

void FrameBufferObject::attachTextureLayer(GLenum attachment, Texture* texture, GLint level, GLint layer)
{
    assert(texture != nullptr);

	bind();

	glFramebufferTextureLayer(m_target, attachment, texture->id(), level, layer);
	CheckGLError();

    attach(new TextureAttachment(texture, attachment, level, layer));
}

void FrameBufferObject::attachRenderBuffer(GLenum attachment, RenderBufferObject* renderBuffer)
{
    assert(renderBuffer != nullptr);

	bind();
	renderBuffer->bind();

	glFramebufferRenderbuffer(m_target, attachment, GL_RENDERBUFFER, renderBuffer->id());
	CheckGLError();

	attach(new RenderBufferAttachment(renderBuffer, attachment));
}

void FrameBufferObject::detach(GLenum attachment)
{
    FrameBufferAttachment* fAttachment = this->attachment(attachment);
    if (!fAttachment)
        return;

    m_attachments.erase(attachment);

    bind();

    if (fAttachment->isTextureAttachment())
    {
        TextureAttachment * tAttachment = fAttachment->asTextureAttachment();
        if (tAttachment->hasLayer())
        {
            glFramebufferTextureLayer(m_target, attachment, 0, tAttachment->level(), tAttachment->layer());
            CheckGLError();
        }
        else
        {
            glFramebufferTexture(m_target, attachment, 0, tAttachment->level());
            CheckGLError();
        }
    }
    else if (fAttachment->isRenderBufferAttachment())
    {
        glFramebufferRenderbuffer(m_target, attachment, GL_RENDERBUFFER, 0);
        CheckGLError();
    }
}

void FrameBufferObject::attach(FrameBufferAttachment* attachment)
{
    assert(attachment != nullptr);

	m_attachments[attachment->attachment()] = attachment;
}

void FrameBufferObject::setReadBuffer(GLenum mode) const
{
	bind(GL_READ_FRAMEBUFFER);

	glReadBuffer(mode);
	CheckGLError();
}

void FrameBufferObject::setDrawBuffer(GLenum mode) const
{
	bind(GL_DRAW_FRAMEBUFFER);

	glDrawBuffer(mode);
	CheckGLError();
}

void FrameBufferObject::setDrawBuffers(GLsizei n, const GLenum* modes) const
{
    assert(modes != nullptr || n == 0);

	bind(GL_DRAW_FRAMEBUFFER);

	glDrawBuffers(n, modes);
	CheckGLError();
}

void FrameBufferObject::setDrawBuffers(const std::vector<GLenum>& modes) const
{
    setDrawBuffers(static_cast<int>(modes.size()), modes.data());
}

void FrameBufferObject::clear(GLbitfield mask)
{
    bind(GL_FRAMEBUFFER);
    glClear(mask);
    CheckGLError();
}

void FrameBufferObject::clearBufferiv(GLenum buffer, GLint drawBuffer, const GLint * value)
{
    bind(GL_FRAMEBUFFER);
    glClearBufferiv(buffer, drawBuffer, value);
    CheckGLError();
}

void FrameBufferObject::clearBufferuiv(GLenum buffer, GLint drawBuffer, const GLuint * value)
{
    bind(GL_FRAMEBUFFER);
    glClearBufferuiv(buffer, drawBuffer, value);
    CheckGLError();
}

void FrameBufferObject::clearBufferfv(GLenum buffer, GLint drawBuffer, const GLfloat * value)
{
    bind(GL_FRAMEBUFFER);
    glClearBufferfv(buffer, drawBuffer, value);
    CheckGLError();
}

void FrameBufferObject::clearBufferfi(GLenum buffer, GLint drawBuffer, GLfloat depth, GLint stencil)
{
    bind(GL_FRAMEBUFFER);
    glClearBufferfi(buffer, drawBuffer, depth, stencil);
    CheckGLError();
}

void FrameBufferObject::clearBuffer(GLenum buffer, GLint drawBuffer, const glm::ivec4 & value)
{
    clearBufferiv(buffer, drawBuffer, glm::value_ptr(value));
}

void FrameBufferObject::clearBuffer(GLenum buffer, GLint drawBuffer, const glm::uvec4 & value)
{
    clearBufferuiv(buffer, drawBuffer, glm::value_ptr(value));
}

void FrameBufferObject::clearBuffer(GLenum buffer, GLint drawBuffer, const glm::vec4 & value)
{
    clearBufferfv(buffer, drawBuffer, glm::value_ptr(value));
}

void FrameBufferObject::colorMask(GLboolean red, GLboolean green, GLboolean blue, GLboolean alpha)
{
    glColorMask(red, green, blue, alpha);
    CheckGLError();
}

void FrameBufferObject::colorMask(const glm::bvec4 & mask)
{
    colorMask(mask[0], mask[1], mask[2], mask[3]);
}

void FrameBufferObject::colorMaski(GLuint buffer, GLboolean red, GLboolean green, GLboolean blue, GLboolean alpha)
{
    glColorMaski(buffer, red, green, blue, alpha);
    CheckGLError();
}

void FrameBufferObject::colorMaski(GLuint buffer, const glm::bvec4 & mask)
{
    colorMaski(buffer, mask[0], mask[1], mask[2], mask[3]);
}

void FrameBufferObject::clearColor(GLfloat red, GLfloat green, GLfloat blue, GLfloat alpha)
{
    glClearColor(red, green, blue, alpha);
    CheckGLError();
}

void FrameBufferObject::clearColor(const glm::vec4 & color)
{
    clearColor(color.r, color.g, color.b, color.a);
}

void FrameBufferObject::clearDepth(GLclampd depth)
{
    glClearDepth(depth);
    CheckGLError();
}

void FrameBufferObject::readPixels(GLint x, GLint y, GLsizei width, GLsizei height, GLenum format, GLenum type, GLvoid * data) const
{
    glBindFramebuffer(GL_READ_FRAMEBUFFER, m_id);
    CheckGLError();

	glReadPixels(x, y, width, height, format, type, data);
	CheckGLError();
}

void FrameBufferObject::readPixels(const std::array<GLint, 4> & rect, GLenum format, GLenum type, GLvoid * data) const
{
    readPixels(rect[0], rect[1], rect[2], rect[3], format, type, data);
}

void FrameBufferObject::readPixels(GLenum readBuffer, const std::array<GLint, 4> & rect, GLenum format, GLenum type, GLvoid * data) const
{
    setReadBuffer(readBuffer);
    readPixels(rect, format, type, data);
}

std::vector<unsigned char> FrameBufferObject::readPixelsToByteArray(const std::array<GLint, 4> & rect, GLenum format, GLenum type) const
{
    int size = imageSizeInBytes(rect[2], rect[3], format, type);
    std::vector<unsigned char> data(size);

    readPixels(rect, format, type, data.data());

    return data;
}

std::vector<unsigned char> FrameBufferObject::readPixelsToByteArray(GLenum readBuffer, const std::array<GLint, 4> & rect, GLenum format, GLenum type) const
{
    setReadBuffer(readBuffer);
    return readPixelsToByteArray(rect, format, type);
}

void FrameBufferObject::readPixelsToBuffer(const std::array<GLint, 4> & rect, GLenum format, GLenum type, Buffer * pbo) const
{
    assert(pbo != nullptr);

	pbo->bind(GL_PIXEL_PACK_BUFFER);
    readPixels(rect, format, type, nullptr);
	pbo->unbind();
}

void FrameBufferObject::blit(GLenum readBuffer, const std::array<GLint, 4> & srcRect, FrameBufferObject * destFbo, GLenum drawBuffer, const std::array<GLint, 4> & destRect, GLbitfield mask, GLenum filter) const
{
    blit(readBuffer, srcRect, destFbo, std::vector<GLenum>{ drawBuffer }, destRect, mask, filter);
}

void FrameBufferObject::blit(GLenum readBuffer, const std::array<GLint, 4> & srcRect, FrameBufferObject * destFbo, const std::vector<GLenum> & drawBuffers, const std::array<GLint, 4> & destRect, GLbitfield mask, GLenum filter) const
{
    glBindFramebuffer(GL_READ_FRAMEBUFFER, m_id);
    CheckGLError();

    setReadBuffer(readBuffer);

    glBindFramebuffer(GL_DRAW_FRAMEBUFFER, destFbo->id());
    CheckGLError();

    destFbo->setDrawBuffers(drawBuffers);

    blit(srcRect, destRect, mask, filter);
}

void FrameBufferObject::blit(GLint srcX0, GLint srcY0, GLint srcX1, GLint srcY1, GLint destX0, GLint destY0, GLint destX1, GLint destY1, GLbitfield mask, GLenum filter)
{
    glBlitFramebuffer(srcX0, srcY0, srcX1, srcY1, destX0, destY0, destX1, destY1, mask, filter);
    CheckGLError();
}

void FrameBufferObject::blit(const std::array<GLint, 4> & srcRect, const std::array<GLint, 4> & destRect, GLbitfield mask, GLenum filter)
{
    blit(srcRect[0], srcRect[1], srcRect[2], srcRect[3], destRect[0], destRect[1], destRect[2], destRect[3], mask, filter);
}

GLenum FrameBufferObject::checkStatus() const
{
	bind();

	GLenum result = glCheckFramebufferStatus(m_target);
	CheckGLError();
	return result;
}

std::string FrameBufferObject::statusString() const
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

void FrameBufferObject::printStatus(bool onlyErrors) const
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
