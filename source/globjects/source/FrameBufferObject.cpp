#include <globjects/FrameBufferObject.h>

#include <cassert>

#include <glbinding/gl/functions.h>

#include <glm/gtc/type_ptr.hpp>

#include <globjects/logging.h>
#include <globjects/ObjectVisitor.h>
#include <globjects/TextureAttachment.h>
#include <globjects/FrameBufferAttachment.h>
#include <globjects/RenderBufferAttachment.h>
#include <globjects/Buffer.h>
#include <globjects/RenderBufferObject.h>
#include <globjects/Texture.h>
#include <globjects/constants.h>
#include "pixelformat.h"

#include "registry/ImplementationRegistry.h"
#include "registry/ObjectRegistry.h"
#include "implementations/AbstractFrameBufferImplementation.h"

#include "Resource.h"

namespace {

const glo::AbstractFrameBufferImplementation & implementation()
{
    return glo::ImplementationRegistry::current().frameBufferImplementation();
}

}

namespace glo
{

FrameBufferObject::FrameBufferObject()
: Object(new FrameBufferObjectResource)
{
}

FrameBufferObject::FrameBufferObject(IDResource * resource)
: Object(resource)
{
}

FrameBufferObject * FrameBufferObject::fromId(gl::GLuint id)
{
    return new FrameBufferObject(new ExternalResource(id));
}

FrameBufferObject * FrameBufferObject::defaultFBO()
{
    return ObjectRegistry::current().defaultFBO();
}

FrameBufferObject::~FrameBufferObject()
{
}

void FrameBufferObject::accept(ObjectVisitor& visitor)
{
	visitor.visitFrameBufferObject(this);
}

void FrameBufferObject::bind(gl::GLenum target) const
{
    gl::glBindFramebuffer(target, id());
}

void FrameBufferObject::unbind(gl::GLenum target)
{
    gl::glBindFramebuffer(target, 0);
}

void FrameBufferObject::setParameter(gl::GLenum pname, gl::GLint param)
{
    implementation().setParameter(this, pname, param);
}

gl::GLint FrameBufferObject::getAttachmentParameter(gl::GLenum attachment, gl::GLenum pname) const
{
    return implementation().getAttachmentParameter(this, attachment, pname);
}

void FrameBufferObject::attachTexture(gl::GLenum attachment, Texture * texture, gl::GLint level)
{
    implementation().attachTexture(this, attachment, texture, level);

    addAttachment(new TextureAttachment(this, attachment, texture, level));
}

void FrameBufferObject::attachTextureLayer(gl::GLenum attachment, Texture * texture, gl::GLint level, gl::GLint layer)
{
    implementation().attachTextureLayer(this, attachment, texture, level, layer);

    addAttachment(new TextureAttachment(this, attachment, texture, level, layer));
}

void FrameBufferObject::attachRenderBuffer(gl::GLenum attachment, RenderBufferObject * renderBuffer)
{
    implementation().attachRenderBuffer(this, attachment, renderBuffer);

    addAttachment(new RenderBufferAttachment(this, attachment, renderBuffer));
}

bool FrameBufferObject::detach(gl::GLenum attachment)
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

void FrameBufferObject::setReadBuffer(gl::GLenum mode) const
{
    implementation().setReadBuffer(this, mode);
}

void FrameBufferObject::setDrawBuffer(gl::GLenum mode) const
{
    implementation().setDrawBuffer(this, mode);
}

void FrameBufferObject::setDrawBuffers(gl::GLsizei n, const gl::GLenum * modes) const
{
    assert(modes != nullptr || n == 0);

    implementation().setDrawBuffers(this, n, modes);
}

void FrameBufferObject::setDrawBuffers(const std::vector<gl::GLenum> & modes) const
{
    setDrawBuffers(static_cast<int>(modes.size()), modes.data());
}

void FrameBufferObject::clear(gl::ClearBufferMask mask)
{
    bind(gl::GL_FRAMEBUFFER);

    gl::glClear(mask);
}

void FrameBufferObject::clearBufferiv(gl::GLenum buffer, gl::GLint drawBuffer, const gl::GLint * value)
{
    bind(gl::GL_FRAMEBUFFER);

    gl::glClearBufferiv(buffer, drawBuffer, value);
}

void FrameBufferObject::clearBufferuiv(gl::GLenum buffer, gl::GLint drawBuffer, const gl::GLuint * value)
{
    bind(gl::GL_FRAMEBUFFER);

    gl::glClearBufferuiv(buffer, drawBuffer, value);
}

void FrameBufferObject::clearBufferfv(gl::GLenum buffer, gl::GLint drawBuffer, const gl::GLfloat * value)
{
    bind(gl::GL_FRAMEBUFFER);

    gl::glClearBufferfv(buffer, drawBuffer, value);
}

void FrameBufferObject::clearBufferfi(gl::GLenum buffer, gl::GLint drawBuffer, gl::GLfloat depth, gl::GLint stencil)
{
    bind(gl::GL_FRAMEBUFFER);

    gl::glClearBufferfi(buffer, drawBuffer, depth, stencil);
}

void FrameBufferObject::clearBuffer(gl::GLenum buffer, gl::GLint drawBuffer, const glm::ivec4 & value)
{
    clearBufferiv(buffer, drawBuffer, glm::value_ptr(value));
}

void FrameBufferObject::clearBuffer(gl::GLenum buffer, gl::GLint drawBuffer, const glm::uvec4 & value)
{
    clearBufferuiv(buffer, drawBuffer, glm::value_ptr(value));
}

void FrameBufferObject::clearBuffer(gl::GLenum buffer, gl::GLint drawBuffer, const glm::vec4 & value)
{
    clearBufferfv(buffer, drawBuffer, glm::value_ptr(value));
}

void FrameBufferObject::colorMask(gl::GLboolean red, gl::GLboolean green, gl::GLboolean blue, gl::GLboolean alpha)
{
    gl::glColorMask(red, green, blue, alpha);
}

void FrameBufferObject::colorMask(const glm::bvec4 & mask)
{
    colorMask(static_cast<gl::GLboolean>(mask[0]), static_cast<gl::GLboolean>(mask[1]), static_cast<gl::GLboolean>(mask[2]), static_cast<gl::GLboolean>(mask[3]));
}

void FrameBufferObject::colorMaski(gl::GLuint buffer, gl::GLboolean red, gl::GLboolean green, gl::GLboolean blue, gl::GLboolean alpha)
{
    gl::glColorMaski(buffer, red, green, blue, alpha);
}

void FrameBufferObject::colorMaski(gl::GLuint buffer, const glm::bvec4 & mask)
{
    colorMaski(buffer, static_cast<gl::GLboolean>(mask[0]), static_cast<gl::GLboolean>(mask[1]), static_cast<gl::GLboolean>(mask[2]), static_cast<gl::GLboolean>(mask[3]));
}

void FrameBufferObject::clearColor(gl::GLfloat red, gl::GLfloat green, gl::GLfloat blue, gl::GLfloat alpha)
{
    gl::glClearColor(red, green, blue, alpha);
}

void FrameBufferObject::clearColor(const glm::vec4 & color)
{
    clearColor(color.r, color.g, color.b, color.a);
}

void FrameBufferObject::clearDepth(gl::GLclampd depth)
{
    gl::glClearDepth(depth);
}

void FrameBufferObject::readPixels(gl::GLint x, gl::GLint y, gl::GLsizei width, gl::GLsizei height, gl::GLenum format, gl::GLenum type, gl::GLvoid * data) const
{
    bind(gl::GL_READ_FRAMEBUFFER);

    gl::glReadPixels(x, y, width, height, format, type, data);
}

void FrameBufferObject::readPixels(const std::array<gl::GLint, 4> & rect, gl::GLenum format, gl::GLenum type, gl::GLvoid * data) const
{
    readPixels(rect[0], rect[1], rect[2], rect[3], format, type, data);
}

void FrameBufferObject::readPixels(gl::GLenum readBuffer, const std::array<gl::GLint, 4> & rect, gl::GLenum format, gl::GLenum type, gl::GLvoid * data) const
{
    setReadBuffer(readBuffer);
    readPixels(rect, format, type, data);
}

std::vector<unsigned char> FrameBufferObject::readPixelsToByteArray(const std::array<gl::GLint, 4> & rect, gl::GLenum format, gl::GLenum type) const
{
    int size = imageSizeInBytes(rect[2], rect[3], format, type);
    std::vector<unsigned char> data(size);

    readPixels(rect, format, type, data.data());

    return data;
}

std::vector<unsigned char> FrameBufferObject::readPixelsToByteArray(gl::GLenum readBuffer, const std::array<gl::GLint, 4> & rect, gl::GLenum format, gl::GLenum type) const
{
    setReadBuffer(readBuffer);
    return readPixelsToByteArray(rect, format, type);
}

void FrameBufferObject::readPixelsToBuffer(const std::array<gl::GLint, 4> & rect, gl::GLenum format, gl::GLenum type, Buffer * pbo) const
{
    assert(pbo != nullptr);

    pbo->bind(gl::GL_PIXEL_PACK_BUFFER);
    readPixels(rect, format, type, nullptr);
    pbo->unbind(gl::GL_PIXEL_PACK_BUFFER);
}

void FrameBufferObject::blit(gl::GLenum readBuffer, const std::array<gl::GLint, 4> & srcRect, FrameBufferObject * destFbo, gl::GLenum drawBuffer, const std::array<gl::GLint, 4> & destRect, gl::ClearBufferMask mask, gl::GLenum filter) const
{
    blit(readBuffer, srcRect, destFbo, std::vector<gl::GLenum>{ drawBuffer }, destRect, mask, filter);
}

void FrameBufferObject::blit(gl::GLenum readBuffer, const std::array<gl::GLint, 4> & srcRect, FrameBufferObject * destFbo, const std::vector<gl::GLenum> & drawBuffers, const std::array<gl::GLint, 4> & destRect, gl::ClearBufferMask mask, gl::GLenum filter) const
{
    setReadBuffer(readBuffer);
    destFbo->setDrawBuffers(drawBuffers);

    bind(gl::GL_READ_FRAMEBUFFER);
    destFbo->bind(gl::GL_DRAW_FRAMEBUFFER);

    blit(srcRect, destRect, mask, filter);
}

void FrameBufferObject::blit(gl::GLint srcX0, gl::GLint srcY0, gl::GLint srcX1, gl::GLint srcY1, gl::GLint destX0, gl::GLint destY0, gl::GLint destX1, gl::GLint destY1, gl::ClearBufferMask mask, gl::GLenum filter)
{
    gl::glBlitFramebuffer(srcX0, srcY0, srcX1, srcY1, destX0, destY0, destX1, destY1, mask, filter);

}

void FrameBufferObject::blit(const std::array<gl::GLint, 4> & srcRect, const std::array<gl::GLint, 4> & destRect, gl::ClearBufferMask mask, gl::GLenum filter)
{
    blit(srcRect[0], srcRect[1], srcRect[2], srcRect[3], destRect[0], destRect[1], destRect[2], destRect[3], mask, filter);
}

gl::GLenum FrameBufferObject::checkStatus() const
{
    return implementation().checkStatus(this);
}

std::string FrameBufferObject::statusString() const
{
    return enumName(checkStatus());
}

void FrameBufferObject::printStatus(bool onlyErrors) const
{
	gl::GLenum status = checkStatus();

	if (onlyErrors && status == gl::GL_FRAMEBUFFER_COMPLETE) return;

	if (status == gl::GL_FRAMEBUFFER_COMPLETE)
	{
        info() << enumName(gl::GL_FRAMEBUFFER_COMPLETE);
	}
	else
	{
		std::stringstream ss;
		ss.flags(std::ios::hex | std::ios::showbase);
        ss << static_cast<unsigned int>(status);

        critical() << enumName(status) << " (" << ss.str() << ")";
	}
}

void FrameBufferObject::addAttachment(FrameBufferAttachment * attachment)
{
    assert(attachment != nullptr);

    m_attachments[attachment->attachment()] = attachment;
}

FrameBufferAttachment * FrameBufferObject::getAttachment(gl::GLenum attachment)
{
	return m_attachments[attachment];
}

std::vector<FrameBufferAttachment*> FrameBufferObject::attachments()
{
	std::vector<FrameBufferAttachment*> attachments;

    for (std::pair<gl::GLenum, ref_ptr<FrameBufferAttachment>> pair: m_attachments)
	{
		attachments.push_back(pair.second);
	}

	return attachments;
}

gl::GLenum FrameBufferObject::objectType() const
{
    return gl::GL_FRAMEBUFFER;
}

} // namespace glo
