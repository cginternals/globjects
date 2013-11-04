#pragma once

#include <string>
#include <vector>
#include <map>

#include <glow/glow.h>
#include <glow/Object.h>

namespace glow 
{

class FrameBufferAttachment;

/** \brief Enables creation of arbitrary render targets that are not directly drawn on the screen.
    
    Different attachments can be added with attachTexture(), attachTexture1D(), 
    attachTexture2D, attachTextureLayer() and attachRenderBuffer() and queried 
    using attachment() and attachments().
    
    To access the default FBO (e.g. if you want to blit an FBO to it), call 
    defaultFBO(). To blit between two FBOs, prepare them with setReadBuffer()
    and setDrawBuffer() and blit with blit(). 

    Draw restrictions can be done with setDrawBuffers(). To read pixels from 
    an FBO direct into RAM, use readPixels() and to read into an OpenGL buffer 
    use readPixelsToBuffer(). To check if an FBO is setup correctly, the status
    can be checked using checkStatus(), statusString() and printStatus().

    \see http://www.opengl.org/wiki/Framebuffer_Object
    \see FrameBufferAttachment
    \see TextureAttachment
    \see RenderBufferAttachment
 */
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

	void attachTexture(GLenum attachment, Texture * texture, GLint level = 0);
	void attachTexture1D(GLenum attachment, Texture * texture, GLint level = 0);
	void attachTexture2D(GLenum attachment, Texture * texture, GLint level = 0);
	void attachTextureLayer(GLenum attachment, Texture * texture, GLint level = 0, GLint layer = 0);
	void attachRenderBuffer(GLenum attachment, RenderBufferObject * renderBuffer);

	void setReadBuffer(GLenum mode);
	void setDrawBuffer(GLenum mode);
	void setDrawBuffers(GLsizei n, const GLenum * modes);
	void setDrawBuffers(const std::vector<GLenum> & modes);

	void readPixels(
        GLint x
    ,   GLint y
    ,   GLsizei width
    ,   GLsizei height
    ,   GLenum format
    ,   GLenum type
    ,   GLvoid * data = nullptr);

	void readPixelsToBuffer(
        GLint x
    ,   GLint y
    ,   GLsizei width
    ,   GLsizei height
    ,   GLenum format
    , GLenum type, Buffer* pbo);


    GLenum checkStatus();
    std::string statusString();
    std::string statusString(GLenum status);
    void printStatus(bool onlyErrors = false);

    FrameBufferAttachment * attachment(GLenum attachment);
    std::vector<FrameBufferAttachment*> attachments();

public:
	static void blit(
        GLint srcX0
    ,   GLint srcY0
    ,   GLint srcX1
    ,   GLint srcY1
    ,   GLint destX0
    ,   GLint destY0
    ,   GLint destX1
    ,   GLint destY1
    ,   GLbitfield mask
    ,   GLenum filter);

protected:
    void attach(FrameBufferAttachment * attachment);

    static GLuint genFrameBuffer();

protected:
	GLenum m_target;
	std::map<GLenum, ref_ptr<FrameBufferAttachment>> m_attachments;

	static FrameBufferObject s_defaultFBO;
};

} // namespace glow
