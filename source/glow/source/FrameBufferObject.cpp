#include <glow/FrameBufferObject.h>

#include <cassert>

#include <glm/gtc/type_ptr.hpp>

#include <glow/logging.h>
#include <glow/Error.h>
#include <glow/ObjectVisitor.h>
#include <glow/TextureAttachment.h>
#include <glow/FrameBufferAttachment.h>
#include <glow/RenderBufferAttachment.h>
#include <glow/Buffer.h>
#include <glow/RenderBufferObject.h>
#include <glow/Texture.h>
#include <glow/constants.h>
#include "pixelformat.h"

#include "registry/ImplementationRegistry.h"
#include "implementations/AbstractFrameBufferImplementation.h"

namespace {

const glow::AbstractFrameBufferImplementation & implementation()
{
    return glow::ImplementationRegistry::current().frameBufferImplementation();
}

}

namespace glow
{

FrameBufferObject::FrameBufferObject()
: Object(genFrameBuffer())
{
}

FrameBufferObject::FrameBufferObject(GLuint id, bool takeOwnership)
: Object(id, takeOwnership)
{
}

FrameBufferObject * FrameBufferObject::fromId(GLuint id, bool takeOwnership)
{
    return new FrameBufferObject(id, takeOwnership);
}

FrameBufferObject * FrameBufferObject::defaultFBO()
{
    // TODO: this is dangerous as the receiver needs to delete this object.
    // better store default objects in the ObjectRegistry
    return new FrameBufferObject(0, false);
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

void FrameBufferObject::bind(GLenum target) const
{
	glBindFramebuffer(target, m_id);
	CheckGLError();
}

void FrameBufferObject::unbind(GLenum target)
{
    glBindFramebuffer(target, 0);
    CheckGLError();
}

void FrameBufferObject::setParameter(GLenum pname, GLint param)
{
    implementation().setParameter(this, pname, param);
}

GLint FrameBufferObject::getAttachmentParameter(GLenum attachment, GLenum pname) const
{
    return implementation().getAttachmentParameter(this, attachment, pname);
}

void FrameBufferObject::attachTexture(GLenum attachment, Texture * texture, GLint level)
{
    implementation().attachTexture(this, attachment, texture, level);

    addAttachment(new TextureAttachment(this, attachment, texture, level));
}

void FrameBufferObject::attachTexture1D(GLenum attachment, Texture * texture, GLint level)
{
    implementation().attachTexture1D(this, attachment, texture, level);

    addAttachment(new TextureAttachment(this, attachment, texture, level));
}

void FrameBufferObject::attachTexture2D(GLenum attachment, Texture* texture, GLint level)
{
    implementation().attachTexture2D(this, attachment, texture, level);

    addAttachment(new TextureAttachment(this, attachment, texture, level));
}

void FrameBufferObject::attachTexture3D(GLenum attachment, Texture * texture, GLint level, GLint layer)
{
    implementation().attachTexture3D(this, attachment, texture, level, layer);

    addAttachment(new TextureAttachment(this, attachment, texture, level, layer));
}

void FrameBufferObject::attachTextureLayer(GLenum attachment, Texture * texture, GLint level, GLint layer)
{
    implementation().attachTextureLayer(this, attachment, texture, level, layer);

    addAttachment(new TextureAttachment(this, attachment, texture, level, layer));
}

void FrameBufferObject::attachRenderBuffer(GLenum attachment, RenderBufferObject * renderBuffer)
{
    implementation().attachRenderBuffer(this, attachment, renderBuffer);

    addAttachment(new RenderBufferAttachment(this, attachment, renderBuffer));
}

bool FrameBufferObject::detach(GLenum attachment)
{
    FrameBufferAttachment * attachmentObject = getAttachment(attachment);

    if (!attachmentObject)
        return false;

    if (attachmentObject->isTextureAttachment())
    {
        TextureAttachment * textureAttachment = attachmentObject->asTextureAttachment();

        if (textureAttachment->hasLayer())
        {
            implementation().attachTextureLayer(this, attachment, nullptr, textureAttachment->level(), textureAttachment->layer());
        }
        else
        {
            implementation().attachTexture(this, attachment, nullptr, textureAttachment->level());
        }
    }
    else if (attachmentObject->isRenderBufferAttachment())
    {
        implementation().attachRenderBuffer(this, attachment, nullptr);
    }

    m_attachments.erase(attachment);

    return true;
}

void FrameBufferObject::setReadBuffer(GLenum mode) const
{
    implementation().setReadBuffer(this, mode);
}

void FrameBufferObject::setDrawBuffer(GLenum mode) const
{
    implementation().setDrawBuffer(this, mode);
}

void FrameBufferObject::setDrawBuffers(GLsizei n, const GLenum * modes) const
{
    assert(modes != nullptr || n == 0);

    implementation().setDrawBuffers(this, n, modes);
}

void FrameBufferObject::setDrawBuffers(const std::vector<GLenum> & modes) const
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
    bind(GL_READ_FRAMEBUFFER);

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
    pbo->unbind(GL_PIXEL_PACK_BUFFER);
}

void FrameBufferObject::blit(GLenum readBuffer, const std::array<GLint, 4> & srcRect, FrameBufferObject * destFbo, GLenum drawBuffer, const std::array<GLint, 4> & destRect, GLbitfield mask, GLenum filter) const
{
    blit(readBuffer, srcRect, destFbo, std::vector<GLenum>{ drawBuffer }, destRect, mask, filter);
}

void FrameBufferObject::blit(GLenum readBuffer, const std::array<GLint, 4> & srcRect, FrameBufferObject * destFbo, const std::vector<GLenum> & drawBuffers, const std::array<GLint, 4> & destRect, GLbitfield mask, GLenum filter) const
{
    setReadBuffer(readBuffer);
    destFbo->setDrawBuffers(drawBuffers);

    bind(GL_READ_FRAMEBUFFER);
    destFbo->bind(GL_DRAW_FRAMEBUFFER);

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
    return implementation().checkStatus(this);
}

std::string FrameBufferObject::statusString() const
{
    return enumName(checkStatus());
}

void FrameBufferObject::printStatus(bool onlyErrors) const
{
	GLenum status = checkStatus();

	if (onlyErrors && status == GL_FRAMEBUFFER_COMPLETE) return;

	if (status == GL_FRAMEBUFFER_COMPLETE)
	{
        info() << enumName(GL_FRAMEBUFFER_COMPLETE);
	}
	else
	{
		std::stringstream ss;
		ss.flags(std::ios::hex | std::ios::showbase);
		ss << status;

        critical() << enumName(status) << " (" << ss.str() << ")";
	}
}

void FrameBufferObject::addAttachment(FrameBufferAttachment * attachment)
{
    assert(attachment != nullptr);

    m_attachments[attachment->attachment()] = attachment;
}

FrameBufferAttachment * FrameBufferObject::getAttachment(GLenum attachment)
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
