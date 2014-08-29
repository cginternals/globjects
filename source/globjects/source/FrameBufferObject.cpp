
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


using namespace gl;

namespace 
{

const glo::AbstractFramebufferImplementation & implementation()
{
    return glo::ImplementationRegistry::current().framebufferImplementation();
}

}


namespace glo
{

void FrameBufferObject::hintBindlessImplementation(BindlessImplementation impl)
{
    ImplementationRegistry::current().initialize(impl);
}
    

FrameBufferObject::FrameBufferObject()
: Object(new FrameBufferObjectResource)
{
}

FrameBufferObject::FrameBufferObject(IDResource * resource)
: Object(resource)
{
}

FrameBufferObject * FrameBufferObject::fromId(GLuint id)
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

void FrameBufferObject::bind(GLenum target) const
{
    glBindFramebuffer(target, id());
}

void FrameBufferObject::unbind(GLenum target)
{
    glBindFramebuffer(target, 0);
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

void FrameBufferObject::clear(ClearBufferMask mask)
{
    bind(GL_FRAMEBUFFER);

    glClear(mask);
}

void FrameBufferObject::clearBufferiv(GLenum buffer, GLint drawBuffer, const GLint * value)
{
    bind(GL_FRAMEBUFFER);

    glClearBufferiv(buffer, drawBuffer, value);
}

void FrameBufferObject::clearBufferuiv(GLenum buffer, GLint drawBuffer, const GLuint * value)
{
    bind(GL_FRAMEBUFFER);

    glClearBufferuiv(buffer, drawBuffer, value);
}

void FrameBufferObject::clearBufferfv(GLenum buffer, GLint drawBuffer, const GLfloat * value)
{
    bind(GL_FRAMEBUFFER);

    glClearBufferfv(buffer, drawBuffer, value);
}

void FrameBufferObject::clearBufferfi(GLenum buffer, GLint drawBuffer, GLfloat depth, GLint stencil)
{
    bind(GL_FRAMEBUFFER);

    glClearBufferfi(buffer, drawBuffer, depth, stencil);
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
}

void FrameBufferObject::colorMask(const glm::bvec4 & mask)
{
    colorMask(static_cast<GLboolean>(mask[0]), static_cast<GLboolean>(mask[1]), static_cast<GLboolean>(mask[2]), static_cast<GLboolean>(mask[3]));
}

void FrameBufferObject::colorMaski(GLuint buffer, GLboolean red, GLboolean green, GLboolean blue, GLboolean alpha)
{
    glColorMaski(buffer, red, green, blue, alpha);
}

void FrameBufferObject::colorMaski(GLuint buffer, const glm::bvec4 & mask)
{
    colorMaski(buffer, static_cast<GLboolean>(mask[0]), static_cast<GLboolean>(mask[1]), static_cast<GLboolean>(mask[2]), static_cast<GLboolean>(mask[3]));
}

void FrameBufferObject::clearColor(GLfloat red, GLfloat green, GLfloat blue, GLfloat alpha)
{
    glClearColor(red, green, blue, alpha);
}

void FrameBufferObject::clearColor(const glm::vec4 & color)
{
    clearColor(color.r, color.g, color.b, color.a);
}

void FrameBufferObject::clearDepth(GLclampd depth)
{
    glClearDepth(depth);
}

void FrameBufferObject::readPixels(GLint x, GLint y, GLsizei width, GLsizei height, GLenum format, GLenum type, GLvoid * data) const
{
    bind(GL_READ_FRAMEBUFFER);

    glReadPixels(x, y, width, height, format, type, data);
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

void FrameBufferObject::blit(GLenum readBuffer, const std::array<GLint, 4> & srcRect, FrameBufferObject * destFbo, GLenum drawBuffer, const std::array<GLint, 4> & destRect, ClearBufferMask mask, GLenum filter) const
{
    blit(readBuffer, srcRect, destFbo, std::vector<GLenum>{ drawBuffer }, destRect, mask, filter);
}

void FrameBufferObject::blit(GLenum readBuffer, const std::array<GLint, 4> & srcRect, FrameBufferObject * destFbo, const std::vector<GLenum> & drawBuffers, const std::array<GLint, 4> & destRect, ClearBufferMask mask, GLenum filter) const
{
    setReadBuffer(readBuffer);
    destFbo->setDrawBuffers(drawBuffers);

    bind(GL_READ_FRAMEBUFFER);
    destFbo->bind(GL_DRAW_FRAMEBUFFER);

    blit(srcRect, destRect, mask, filter);
}

void FrameBufferObject::blit(GLint srcX0, GLint srcY0, GLint srcX1, GLint srcY1, GLint destX0, GLint destY0, GLint destX1, GLint destY1, ClearBufferMask mask, GLenum filter)
{
    glBlitFramebuffer(srcX0, srcY0, srcX1, srcY1, destX0, destY0, destX1, destY1, mask, filter);

}

void FrameBufferObject::blit(const std::array<GLint, 4> & srcRect, const std::array<GLint, 4> & destRect, ClearBufferMask mask, GLenum filter)
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
        ss << static_cast<unsigned int>(status);

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

} // namespace glo
