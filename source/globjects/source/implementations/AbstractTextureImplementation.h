
#pragma once

#include <glbinding/gl/types.h>

#include <globjects/Texture.h>


namespace globjects
{


class AbstractTextureImplementation
{
public:
    AbstractTextureImplementation();
    virtual ~AbstractTextureImplementation();

    static AbstractTextureImplementation * get(Texture::BindlessImplementation impl =
        Texture::BindlessImplementation::DirectStateAccessARB);

    virtual gl::GLuint create(gl::GLenum target) const = 0;
    virtual void destroy(gl::GLuint id) const = 0;

    virtual void bindActive(const Texture * texture, gl::GLuint unit) const = 0;
    virtual void unbindActive(const Texture * texture, gl::GLuint unit) const = 0;

    virtual void generateMipMap(const Texture * texture) const = 0;

    virtual void setParameter(const Texture * texture, gl::GLenum name, gl::GLint value) const = 0;
    virtual void setParameter(const Texture * texture, gl::GLenum name, gl::GLfloat value) const = 0;
    virtual void setParameter(const Texture * texture, gl::GLenum name, const glm::vec4 & value) const = 0;

    virtual gl::GLint getParameter(const Texture * texture, gl::GLenum name) const = 0;
    virtual gl::GLint getLevelParameter(const Texture * texture, gl::GLint level, gl::GLenum name) const = 0;

    virtual void image1D(const Texture * texture, gl::GLint level, gl::GLenum internalFormat, gl::GLsizei width, gl::GLint border, gl::GLenum format, gl::GLenum type, const gl::GLvoid * data) const  = 0;
    virtual void compressedImage1D(const Texture * texture, gl::GLint level, gl::GLenum internalFormat, gl::GLsizei width, gl::GLint border, gl::GLsizei imageSize, const gl::GLvoid * data) const  = 0;
    virtual void subImage1D(const Texture * texture, gl::GLint level, gl::GLint xOffset, gl::GLsizei width, gl::GLenum format, gl::GLenum type, const gl::GLvoid * data) const  = 0;

    virtual void image2D(const Texture * texture, gl::GLint level, gl::GLenum internalFormat, gl::GLsizei width, gl::GLsizei height, gl::GLint border, gl::GLenum format, gl::GLenum type, const gl::GLvoid * data) const  = 0;
    virtual void compressedImage2D(const Texture * texture, gl::GLint level, gl::GLenum internalFormat, gl::GLsizei width, gl::GLsizei height, gl::GLint border, gl::GLsizei imageSize, const gl::GLvoid * data) const  = 0;
    virtual void subImage2D(const Texture * texture, gl::GLint level, gl::GLint xOffset, gl::GLint yOffset, gl::GLsizei width, gl::GLsizei height, gl::GLenum format, gl::GLenum type, const gl::GLvoid * data) const  = 0;

    virtual void image3D(const Texture * texture, gl::GLint level, gl::GLenum internalFormat, gl::GLsizei width, gl::GLsizei height, gl::GLsizei depth, gl::GLint border, gl::GLenum format, gl::GLenum type, const gl::GLvoid * data) const  = 0;
    virtual void compressedImage3D(const Texture * texture, gl::GLint level, gl::GLenum internalFormat, gl::GLsizei width, gl::GLsizei height, gl::GLsizei depth, gl::GLint border, gl::GLsizei imageSize, const gl::GLvoid * data) const  = 0;
    virtual void subImage3D(const Texture * texture, gl::GLint level, gl::GLint xOffset, gl::GLint yOffset, gl::GLint zOffset, gl::GLsizei width, gl::GLsizei height, gl::GLsizei depth, gl::GLenum format, gl::GLenum type, const gl::GLvoid * data) const  = 0;

    virtual void image2DMultisample(const Texture * texture, gl::GLsizei samples, gl::GLenum internalFormat, gl::GLsizei width, gl::GLsizei height, gl::GLboolean fixedSamplesLocations) const  = 0;
    virtual void image3DMultisample(const Texture * texture, gl::GLsizei samples, gl::GLenum internalFormat, gl::GLsizei width, gl::GLsizei height, gl::GLsizei depth, gl::GLboolean fixedSamplesLocations) const  = 0;

    virtual void cubeMapImage(const Texture * texture, gl::GLint level, gl::GLenum internalFormat, gl::GLsizei width, gl::GLsizei height, gl::GLint border, gl::GLenum format, gl::GLenum type, const gl::GLvoid * data) const  = 0;

    virtual void texBuffer(const Texture * texture, const gl::GLenum internalFormat, Buffer * buffer) const = 0;
    virtual void texBufferRange(const Texture * texture, const gl::GLenum internalFormat, Buffer * buffer, const gl::GLintptr offset, const gl::GLsizeiptr size) const = 0;

    virtual void pageCommitment(const Texture * texture, const gl::GLint level, const gl::GLint xOffset, const gl::GLint yOffset, const gl::GLint zOffset, const gl::GLsizei width, const gl::GLsizei height, const gl::GLsizei depth, const gl::GLboolean commit) const = 0;
};


} // namespace globjects
