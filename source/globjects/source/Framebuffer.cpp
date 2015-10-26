
#include <globjects/Framebuffer.h>

#include <cassert>

#include <glbinding/gl/functions.h>
#include <glbinding/gl/enum.h>
#include <glbinding/Meta.h>

#include <glm/gtc/type_ptr.hpp>

#include <globjects/ObjectVisitor.h>
#include <globjects/AttachedTexture.h>
#include <globjects/FramebufferAttachment.h>
#include <globjects/AttachedRenderbuffer.h>
#include "pixelformat.h"

#include "registry/ImplementationRegistry.h"
#include "registry/ObjectRegistry.h"

#include "implementations/AbstractFramebufferImplementation.h"

#include "Resource.h"

using namespace gl;

namespace 
{

const globjects::AbstractFramebufferImplementation & implementation()
{
    return globjects::ImplementationRegistry::current().framebufferImplementation();
}

}


namespace globjects
{

void Framebuffer::hintBindlessImplementation(const BindlessImplementation impl)
{
    ImplementationRegistry::current().initialize(impl);
}
    

Framebuffer::Framebuffer()
: Object(new FrameBufferObjectResource)
{
}

Framebuffer::Framebuffer(IDResource * resource)
: Object(resource)
{
}

Framebuffer * Framebuffer::fromId(const GLuint id)
{
    return new Framebuffer(new ExternalResource(id));
}

Framebuffer * Framebuffer::defaultFBO()
{
    return ObjectRegistry::current().defaultFBO();
}

Framebuffer::~Framebuffer()
{
}

void Framebuffer::accept(ObjectVisitor & visitor)
{
    visitor.visitFrameBufferObject(this);
}

void Framebuffer::bind() const
{
    glBindFramebuffer(GL_FRAMEBUFFER, id());
}

void Framebuffer::bind(const GLenum target) const
{
    glBindFramebuffer(target, id());
}

void Framebuffer::unbind()
{
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void Framebuffer::unbind(const GLenum target)
{
    glBindFramebuffer(target, 0);
}

void Framebuffer::setParameter(const GLenum pname, const GLint param)
{
    implementation().setParameter(this, pname, param);
}

GLint Framebuffer::getAttachmentParameter(const GLenum attachment, const GLenum pname) const
{
    return implementation().getAttachmentParameter(this, attachment, pname);
}

void Framebuffer::attachTexture(const GLenum attachment, Texture * texture, const GLint level)
{
    implementation().attachTexture(this, attachment, texture, level);

    addAttachment(new AttachedTexture(this, attachment, texture, level));
}

void Framebuffer::attachTextureLayer(const GLenum attachment, Texture * texture, const GLint level, const GLint layer)
{
    implementation().attachTextureLayer(this, attachment, texture, level, layer);

    addAttachment(new AttachedTexture(this, attachment, texture, level, layer));
}

void Framebuffer::attachRenderBuffer(const GLenum attachment, Renderbuffer * renderBuffer)
{
    implementation().attachRenderBuffer(this, attachment, renderBuffer);

    addAttachment(new AttachedRenderbuffer(this, attachment, renderBuffer));
}

bool Framebuffer::detach(const GLenum attachment)
{
    FramebufferAttachment * attachmentObject = getAttachment(attachment);

    if (!attachmentObject)
        return false;

    if (attachmentObject->isTextureAttachment())
    {
        AttachedTexture * textureAttachment = attachmentObject->asTextureAttachment();

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

void Framebuffer::setReadBuffer(const GLenum mode) const
{
    implementation().setReadBuffer(this, mode);
}

void Framebuffer::setDrawBuffer(const GLenum mode) const
{
    implementation().setDrawBuffer(this, mode);
}

void Framebuffer::setDrawBuffers(const GLsizei n, const GLenum * modes) const
{
    assert(modes != nullptr || n == 0);

    implementation().setDrawBuffers(this, n, modes);
}

void Framebuffer::setDrawBuffers(const std::vector<GLenum> & modes) const
{
    setDrawBuffers(static_cast<int>(modes.size()), modes.data());
}

void Framebuffer::clear(const ClearBufferMask mask)
{
    bind();

    glClear(mask);
}

void Framebuffer::clearBufferiv(const GLenum buffer, const GLint drawBuffer, const GLint * value)
{
    bind();

    glClearBufferiv(buffer, drawBuffer, value);
}

void Framebuffer::clearBufferuiv(const GLenum buffer, const GLint drawBuffer, const GLuint * value)
{
    bind();

    glClearBufferuiv(buffer, drawBuffer, value);
}

void Framebuffer::clearBufferfv(const GLenum buffer, const GLint drawBuffer, const GLfloat * value)
{
    bind();

    glClearBufferfv(buffer, drawBuffer, value);
}

void Framebuffer::clearBufferfi(const GLenum buffer, const GLint drawBuffer, const GLfloat depth, const GLint stencil)
{
    bind();

    glClearBufferfi(buffer, drawBuffer, depth, stencil);
}

void Framebuffer::clearBuffer(const GLenum buffer, const GLint drawBuffer, const glm::ivec4 & value)
{
    clearBufferiv(buffer, drawBuffer, glm::value_ptr(value));
}

void Framebuffer::clearBuffer(const GLenum buffer, const GLint drawBuffer, const glm::uvec4 & value)
{
    clearBufferuiv(buffer, drawBuffer, glm::value_ptr(value));
}

void Framebuffer::clearBuffer(const GLenum buffer, const GLint drawBuffer, const glm::vec4 & value)
{
    clearBufferfv(buffer, drawBuffer, glm::value_ptr(value));
}

void Framebuffer::colorMask(const GLboolean red, const GLboolean green, const GLboolean blue, const GLboolean alpha)
{
    glColorMask(red, green, blue, alpha);
}

void Framebuffer::colorMask(const glm::bvec4 & mask)
{
    colorMask(static_cast<GLboolean>(mask[0]), static_cast<GLboolean>(mask[1]), static_cast<GLboolean>(mask[2]), static_cast<GLboolean>(mask[3]));
}

void Framebuffer::colorMaski(const GLuint buffer, const GLboolean red, const GLboolean green, const GLboolean blue, const GLboolean alpha)
{
    glColorMaski(buffer, red, green, blue, alpha);
}

void Framebuffer::colorMaski(const GLuint buffer, const glm::bvec4 & mask)
{
    colorMaski(buffer, static_cast<GLboolean>(mask[0]), static_cast<GLboolean>(mask[1]), static_cast<GLboolean>(mask[2]), static_cast<GLboolean>(mask[3]));
}

void Framebuffer::clearColor(const GLfloat red, const GLfloat green, const GLfloat blue, const GLfloat alpha)
{
    glClearColor(red, green, blue, alpha);
}

void Framebuffer::clearColor(const glm::vec4 & color)
{
    clearColor(color.r, color.g, color.b, color.a);
}

void Framebuffer::clearDepth(const GLclampd depth)
{
    glClearDepth(depth);
}

void Framebuffer::readPixels(const GLint x, const GLint y, const GLsizei width, const GLsizei height, const GLenum format, const GLenum type, GLvoid * data) const
{
    bind(GL_READ_FRAMEBUFFER);

    glReadPixels(x, y, width, height, format, type, data);
}

void Framebuffer::readPixels(const std::array<GLint, 4> & rect, const GLenum format, const GLenum type, GLvoid * data) const
{
    readPixels(rect[0], rect[1], rect[2], rect[3], format, type, data);
}

void Framebuffer::readPixels(const GLenum readBuffer, const std::array<GLint, 4> & rect, const GLenum format, const GLenum type, GLvoid * data) const
{
    setReadBuffer(readBuffer);
    readPixels(rect, format, type, data);
}

std::vector<unsigned char> Framebuffer::readPixelsToByteArray(const std::array<GLint, 4> & rect, const GLenum format, const GLenum type) const
{
    int size = imageSizeInBytes(rect[2], rect[3], 1, format, type);
    std::vector<unsigned char> data(size);

    readPixels(rect, format, type, data.data());

    return data;
}

std::vector<unsigned char> Framebuffer::readPixelsToByteArray(GLenum readBuffer, const std::array<GLint, 4> & rect, GLenum format, GLenum type) const
{
    setReadBuffer(readBuffer);
    return readPixelsToByteArray(rect, format, type);
}

void Framebuffer::readPixelsToBuffer(const std::array<GLint, 4> & rect, GLenum format, GLenum type, Buffer * pbo) const
{
    assert(pbo != nullptr);

    pbo->bind(GL_PIXEL_PACK_BUFFER);
    readPixels(rect, format, type, nullptr);
    pbo->unbind(GL_PIXEL_PACK_BUFFER);
}

void Framebuffer::blit(GLenum readBuffer, const std::array<GLint, 4> & srcRect, Framebuffer * destFbo, GLenum drawBuffer, const std::array<GLint, 4> & destRect, ClearBufferMask mask, GLenum filter) const
{
    blit(readBuffer, srcRect, destFbo, std::vector<GLenum>{ drawBuffer }, destRect, mask, filter);
}

void Framebuffer::blit(GLenum readBuffer, const std::array<GLint, 4> & srcRect, Framebuffer * destFbo, const std::vector<GLenum> & drawBuffers, const std::array<GLint, 4> & destRect, ClearBufferMask mask, GLenum filter) const
{
    bind(GL_READ_FRAMEBUFFER);
    destFbo->bind(GL_DRAW_FRAMEBUFFER);

    setReadBuffer(readBuffer);
    destFbo->setDrawBuffers(drawBuffers);

    blit(srcRect, destRect, mask, filter);
}

void Framebuffer::blit(GLint srcX0, GLint srcY0, GLint srcX1, GLint srcY1, GLint destX0, GLint destY0, GLint destX1, GLint destY1, ClearBufferMask mask, GLenum filter)
{
    glBlitFramebuffer(srcX0, srcY0, srcX1, srcY1, destX0, destY0, destX1, destY1, mask, filter);

}

void Framebuffer::blit(const std::array<GLint, 4> & srcRect, const std::array<GLint, 4> & destRect, ClearBufferMask mask, GLenum filter)
{
    blit(srcRect[0], srcRect[1], srcRect[2], srcRect[3], destRect[0], destRect[1], destRect[2], destRect[3], mask, filter);
}

GLenum Framebuffer::checkStatus() const
{
    return implementation().checkStatus(this);
}

std::string Framebuffer::statusString() const
{
    return glbinding::Meta::getString(checkStatus());
}

void Framebuffer::printStatus(bool onlyErrors) const
{
	GLenum status = checkStatus();

	if (onlyErrors && status == GL_FRAMEBUFFER_COMPLETE) return;

	if (status == GL_FRAMEBUFFER_COMPLETE)
	{
        info() << glbinding::Meta::getString(GL_FRAMEBUFFER_COMPLETE);
	}
	else
	{
		std::stringstream ss;
		ss.flags(std::ios::hex | std::ios::showbase);
        ss << static_cast<unsigned int>(status);

        critical() << glbinding::Meta::getString(status) << " (" << ss.str() << ")";
	}
}

void Framebuffer::addAttachment(FramebufferAttachment * attachment)
{
    assert(attachment != nullptr);

    m_attachments[attachment->attachment()] = attachment;
}

FramebufferAttachment * Framebuffer::getAttachment(GLenum attachment)
{
	return m_attachments[attachment];
}

std::vector<FramebufferAttachment*> Framebuffer::attachments()
{
	std::vector<FramebufferAttachment*> attachments;

    for (std::pair<GLenum, ref_ptr<FramebufferAttachment>> pair: m_attachments)
	{
		attachments.push_back(pair.second);
	}

	return attachments;
}

GLenum Framebuffer::objectType() const
{
    return GL_FRAMEBUFFER;
}

} // namespace globjects
