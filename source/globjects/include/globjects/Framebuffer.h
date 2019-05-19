
#pragma once


#include <map>
#include <string>
#include <vector>
#include <array>
#include <memory>

#include <glm/fwd.hpp>

#include <globjects/globjects_api.h>
#include <globjects/Object.h>
#include <globjects/base/Instantiator.h>


#ifdef GLOBJECTS_USE_EIGEN
#include <Eigen/Dense>
#endif


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
class GLOBJECTS_API Framebuffer : public Object, public Instantiator<Framebuffer>
{
public:
    enum class BindlessImplementation
    {
        Legacy,
        DirectStateAccessEXT,
        DirectStateAccessARB
    };

    static void hintBindlessImplementation(BindlessImplementation impl);


public:
    Framebuffer();

    virtual ~Framebuffer();

    static std::unique_ptr<Framebuffer> fromId(gl::GLuint id);

    static std::unique_ptr<Framebuffer> defaultFBO();

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

    void clearBuffer(gl::GLenum buffer, gl::GLint drawBuffer, const gl::GLint * value);
    void clearBuffer(gl::GLenum buffer, gl::GLint drawBuffer, const gl::GLuint * value);
    void clearBuffer(gl::GLenum buffer, gl::GLint drawBuffer, const gl::GLfloat * value);
    void clearBuffer(gl::GLenum buffer, gl::GLfloat depth, gl::GLint stencil, gl::GLint drawBuffer = 0);

    void clearBuffer(gl::GLenum buffer, gl::GLint drawBuffer, const glm::ivec4 & value);
    void clearBuffer(gl::GLenum buffer, gl::GLint drawBuffer, const glm::uvec4 & value);
    void clearBuffer(gl::GLenum buffer, gl::GLint drawBuffer, const glm::vec4 & value);
    void clearBuffer(gl::GLenum buffer, gl::GLint drawBuffer, int value);
    void clearBuffer(gl::GLenum buffer, gl::GLint drawBuffer, float value);

#ifdef GLOBJECTS_USE_EIGEN
    void clearBuffer(gl::GLenum buffer, gl::GLint drawBuffer, const Eigen::Vector4i & value);
    void clearBuffer(gl::GLenum buffer, gl::GLint drawBuffer, const Eigen::Matrix<uint, 4, 1> & value);
    void clearBuffer(gl::GLenum buffer, gl::GLint drawBuffer, const Eigen::Vector4f & value);
#endif

    static void colorMask(gl::GLboolean red, gl::GLboolean green, gl::GLboolean blue, gl::GLboolean alpha);
    static void colorMask(const glm::bvec4 & mask);
    static void colorMaski(gl::GLuint buffer, gl::GLboolean red, gl::GLboolean green, gl::GLboolean blue, gl::GLboolean alpha);
    static void colorMaski(gl::GLuint buffer, const glm::bvec4 & mask);
    static void clearColor(gl::GLfloat red, gl::GLfloat green, gl::GLfloat blue, gl::GLfloat alpha);
    static void clearColor(const glm::vec4 & color);
    static void clearDepth(gl::GLdouble depth);

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
    Framebuffer(std::unique_ptr<IDResource> && resource);

    void addAttachment(std::unique_ptr<FramebufferAttachment> && attachment);


protected:
    std::map<gl::GLenum, std::unique_ptr<FramebufferAttachment>> m_attachments;
};


} // namespace globjects
