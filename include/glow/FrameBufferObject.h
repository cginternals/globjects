#pragma once

#include <string>
#include <vector>
#include <map>

#include <glow/glow.h>
#include <glow/Object.h>
#include <glow/Texture.h>
#include <glow/Buffer.h>
#include <glow/RenderBufferObject.h>


// http://www.opengl.org/wiki/Framebuffer_Object

namespace glow {

class GLOW_API FrameBufferAttachment : public Referenced
{
public:
	FrameBufferAttachment(GLenum attachment);

	GLenum attachment() const;

	virtual bool isTextureAttachment() const;
	virtual bool isRenderBufferAttachment() const;

	std::string attachmentString() const;
protected:
	GLenum m_attachment;
};

class GLOW_API TextureAttachment : public FrameBufferAttachment
{
public:
	TextureAttachment(Texture * texture, GLenum attachment);

	virtual bool isTextureAttachment() const;
	Texture* texture();
protected:
	ref_ptr<Texture> m_texture;
};

class GLOW_API RenderBufferAttachment : public FrameBufferAttachment
{
public:
	RenderBufferAttachment(RenderBufferObject* renderBuffer, GLenum attachment);

	virtual bool isRenderBufferAttachment() const;
	RenderBufferObject* renderBuffer();
protected:
	ref_ptr<RenderBufferObject> m_renderBuffer;
};


class GLOW_API FrameBufferObject : public Object
{
public:
	FrameBufferObject();
	FrameBufferObject(GLuint id, bool ownsGLObject = true);
	virtual ~FrameBufferObject();

	static FrameBufferObject* defaultFBO();

	virtual void accept(ObjectVisitor& visitor);

	void bind();
	void bind(GLenum target);
	void unbind();

	void setParameter(GLenum pname, GLint param);

	void attachTexture(GLenum attachment, Texture* texture, GLint level = 0);
	void attachTexture1D(GLenum attachment, Texture* texture, GLint level = 0);
	void attachTexture2D(GLenum attachment, Texture* texture, GLint level = 0);
	void attachTextureLayer(GLenum attachment, Texture* texture, GLint level = 0, GLint layer = 0);
	void attachRenderBuffer(GLenum attachment, RenderBufferObject* renderBuffer);

	void setReadBuffer(GLenum mode);
	void setDrawBuffer(GLenum mode);
	void setDrawBuffers(GLsizei n, const GLenum* modes);

	void readPixels(GLint x, GLint y, GLsizei width, GLsizei height, GLenum format, GLenum type, GLvoid* data = nullptr);
	void readPixelsToBuffer(GLint x, GLint y, GLsizei width, GLsizei height, GLenum format, GLenum type, Buffer* pbo);

	static void blit(GLint srcX0, GLint srcY0, GLint srcX1, GLint srcY1, GLint destX0, GLint destY0, GLint destX1, GLint destY1, GLbitfield mask, GLenum filter);

	GLenum checkStatus();
	std::string statusString();
	std::string statusString(GLenum status);
	void printStatus(bool onlyErrors = false);

	FrameBufferAttachment* attachment(GLenum attachment);
	std::vector<FrameBufferAttachment*> attachments();
protected:
	GLenum m_target;
	std::map<GLenum, ref_ptr<FrameBufferAttachment>> m_attachments;

	void attach(FrameBufferAttachment* attachment);

	static GLuint genFrameBuffer();

	static FrameBufferObject _defaultFBO;
};

} // namespace glow
