#pragma once

#include <map>
#include <string>
#include <vector>
#include <array>

#include <glm/fwd.hpp>

#include <globjects/base/ref_ptr.h>

#include <globjects/globjects_api.h>
#include <globjects/Object.h>


namespace globjects 
{

class FramebufferAttachment;
class Renderbuffer;
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
class GLOBJECTS_API Framebuffer : public Object
{
public:
    enum class BindlessImplementation
    {
        DirectStateAccessARB
    ,   DirectStateAccessEXT
    ,   Legacy
    };

    static void hintBindlessImplementation(BindlessImplementation impl);

public:
    Framebuffer();
    static Framebuffer * fromId(gl::GLuint id);

    static Framebuffer * defaultFBO();

    virtual void accept(ObjectVisitor& visitor) override;

    /** uses GL_FRAMEBUFFER as target
    */
    void bind() const;
    void bind(gl::GLenum target) const;

    /** uses GL_FRAMEBUFFER as target
    */
    static void unbind();
    static void unbind(gl::GLenum target);

    void setParameter(gl::GLenum pname, gl::GLint param);
    gl::GLint getAttachmentParameter(gl::GLenum attachment, gl::GLenum pname) const;

    void attachTexture(gl::GLenum attachment, Texture * texture, gl::GLint level = 0);
    void attachTextureLayer(gl::GLenum attachment, Texture * texture, gl::GLint level = 0, gl::GLint layer = 0);
    void attachRenderBuffer(gl::GLenum attachment, Renderbuffer * renderBuffer);

    bool detach(gl::GLenum attachment);

    void setReadBuffer(gl::GLenum mode) const;
    void setDrawBuffer(gl::GLenum mode) const;
    void setDrawBuffers(gl::GLsizei n, const gl::GLenum * modes) const;
    void setDrawBuffers(const std::vector<gl::GLenum> & modes) const;

    void clear(gl::ClearBufferMask mask);

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

    FramebufferAttachment * getAttachment(gl::GLenum attachment);
    std::vector<FramebufferAttachment*> attachments();

    void blit(gl::GLenum readBuffer, const std::array<gl::GLint, 4> & srcRect, Framebuffer * destFbo, gl::GLenum drawBuffer, const std::array<gl::GLint, 4> & destRect, gl::ClearBufferMask mask, gl::GLenum filter) const;
    void blit(gl::GLenum readBuffer, const std::array<gl::GLint, 4> & srcRect, Framebuffer * destFbo, const std::vector<gl::GLenum> & drawBuffers, const std::array<gl::GLint, 4> & destRect, gl::ClearBufferMask mask, gl::GLenum filter) const;

    virtual gl::GLenum objectType() const override;

protected:
    Framebuffer(IDResource * resource);
    virtual ~Framebuffer();

    void addAttachment(FramebufferAttachment * attachment);

    static void blit(gl::GLint srcX0, gl::GLint srcY0, gl::GLint srcX1, gl::GLint srcY1, gl::GLint destX0, gl::GLint destY0, gl::GLint destX1, gl::GLint destY1, gl::ClearBufferMask mask, gl::GLenum filter);
    static void blit(const std::array<gl::GLint, 4> & srcRect, const std::array<gl::GLint, 4> & destRect, gl::ClearBufferMask mask, gl::GLenum filter);

protected:
	std::map<gl::GLenum, ref_ptr<FramebufferAttachment>> m_attachments;
};

} // namespace globjects
