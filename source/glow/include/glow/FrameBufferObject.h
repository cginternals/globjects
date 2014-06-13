#pragma once

#include <map>
#include <string>
#include <vector>
#include <array>

#include <glbinding/constants.h>

#include <glm/vec4.hpp>

#include <glowbase/ref_ptr.h>

#include <glow/glow_api.h>
#include <glow/Object.h>

namespace glow 
{

class FrameBufferAttachment;
class RenderBufferObject;
class Texture;
class Buffer;

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
    static FrameBufferObject * fromId(gl::GLuint id, bool takeOwnership = false);

    static FrameBufferObject * defaultFBO();

    virtual void accept(ObjectVisitor& visitor) override;

    void bind(gl::GLenum target = gl::FRAMEBUFFER) const;
    static void unbind(gl::GLenum target = gl::FRAMEBUFFER);

	void setParameter(gl::GLenum pname, gl::GLint param);
    gl::GLint getAttachmentParameter(gl::GLenum attachment, gl::GLenum pname) const;

	void attachTexture(gl::GLenum attachment, Texture * texture, gl::GLint level = 0);
	void attachTexture1D(gl::GLenum attachment, Texture * texture, gl::GLint level = 0);
	void attachTexture2D(gl::GLenum attachment, Texture * texture, gl::GLint level = 0);
    void attachTexture3D(gl::GLenum attachment, Texture * texture, gl::GLint level = 0, gl::GLint layer = 0);
	void attachTextureLayer(gl::GLenum attachment, Texture * texture, gl::GLint level = 0, gl::GLint layer = 0);
	void attachRenderBuffer(gl::GLenum attachment, RenderBufferObject * renderBuffer);

    bool detach(gl::GLenum attachment);

    void setReadBuffer(gl::GLenum mode) const;
    void setDrawBuffer(gl::GLenum mode) const;
    void setDrawBuffers(gl::GLsizei n, const gl::GLenum * modes) const;
    void setDrawBuffers(const std::vector<gl::GLenum> & modes) const;

    void clear(gl::GLbitfield mask);

    void clearBufferiv(gl::GLenum buffer, gl::GLint drawBuffer, const gl::GLint * value);
    void clearBufferuiv(gl::GLenum buffer, gl::GLint drawBuffer, const gl::GLuint * value);
    void clearBufferfv(gl::GLenum buffer, gl::GLint drawBuffer, const gl::GLfloat * value);
    void clearBufferfi(gl::GLenum buffer, gl::GLint drawBuffer, gl::GLfloat depth, gl::GLint stencil);

    void clearBuffer(gl::GLenum buffer, gl::GLint drawBuffer, const glm::ivec4 & value);
    void clearBuffer(gl::GLenum buffer, gl::GLint drawBuffer, const glm::uvec4 & value);
    void clearBuffer(gl::GLenum buffer, gl::GLint drawBuffer, const glm::vec4 & value);

    static void colorMask(gl::GLboolean red, gl::GLboolean green, gl::GLboolean blue, gl::GLboolean alpha);
    static void colorMask(const glm::bvec4 & mask);
    static void colorMaski(gl::GLuint buffer, gl::GLboolean red, gl::GLboolean green, gl::GLboolean blue, gl::GLboolean alpha);
    static void colorMaski(gl::GLuint buffer, const glm::bvec4 & mask);
    static void clearColor(gl::GLfloat red, gl::GLfloat green, gl::GLfloat blue, gl::GLfloat alpha);
    static void clearColor(const glm::vec4 & color);
    static void clearDepth(gl::GLclampd depth);

    void readPixels(gl::GLint x, gl::GLint y, gl::GLsizei width, gl::GLsizei height, gl::GLenum format, gl::GLenum type, gl::GLvoid * data = nullptr) const;
    void readPixels(const std::array<gl::GLint, 4> & rect, gl::GLenum format, gl::GLenum type, gl::GLvoid * data = nullptr) const;
    void readPixels(gl::GLenum readBuffer, const std::array<gl::GLint, 4> & rect, gl::GLenum format, gl::GLenum type, gl::GLvoid * data = nullptr) const;
    std::vector<unsigned char> readPixelsToByteArray(const std::array<gl::GLint, 4> & rect, gl::GLenum format, gl::GLenum type) const;
    std::vector<unsigned char> readPixelsToByteArray(gl::GLenum readBuffer, const std::array<gl::GLint, 4> & rect, gl::GLenum format, gl::GLenum type) const;
    void readPixelsToBuffer(const std::array<gl::GLint, 4> & rect, gl::GLenum format, gl::GLenum type, Buffer * pbo) const;

    gl::GLenum checkStatus() const;
    std::string statusString() const;
    void printStatus(bool onlyErrors = false) const;

    FrameBufferAttachment * getAttachment(gl::GLenum attachment);
    std::vector<FrameBufferAttachment*> attachments();

    void blit(gl::GLenum readBuffer, const std::array<gl::GLint, 4> & srcRect, FrameBufferObject * destFbo, gl::GLenum drawBuffer, const std::array<gl::GLint, 4> & destRect, gl::GLbitfield mask, gl::GLenum filter) const;
    void blit(gl::GLenum readBuffer, const std::array<gl::GLint, 4> & srcRect, FrameBufferObject * destFbo, const std::vector<gl::GLenum> & drawBuffers, const std::array<gl::GLint, 4> & destRect, gl::GLbitfield mask, gl::GLenum filter) const;
protected:
    FrameBufferObject(gl::GLuint id, bool takeOwnership);
    virtual ~FrameBufferObject();

    void addAttachment(FrameBufferAttachment * attachment);

    static gl::GLuint genFrameBuffer();

    static void blit(gl::GLint srcX0, gl::GLint srcY0, gl::GLint srcX1, gl::GLint srcY1, gl::GLint destX0, gl::GLint destY0, gl::GLint destX1, gl::GLint destY1, gl::GLbitfield mask, gl::GLenum filter);
    static void blit(const std::array<gl::GLint, 4> & srcRect, const std::array<gl::GLint, 4> & destRect, gl::GLbitfield mask, gl::GLenum filter);
protected:
	std::map<gl::GLenum, ref_ptr<FrameBufferAttachment>> m_attachments;
};

} // namespace glow
