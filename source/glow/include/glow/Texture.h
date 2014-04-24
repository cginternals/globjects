#pragma once

#include <GL/glew.h>

#include <vector>

#include <glm/glm.hpp>

#include <glow/glow_api.h>
#include <glow/Object.h>
#include <glow/TextureHandle.h>

namespace glow {

class Buffer;

/** \brief Wraps OpenGL texture objects.
 * A Texture provides both interfaces to bind them for the OpenGL pipeline:
 * binding and bindless texture. Bindless textures are only available if the
 * graphics driver supports them.
 *
 * \see http://www.opengl.org/wiki/Texture
 * \see http://www.opengl.org/registry/specs/NV/bindless_texture.txt
 */
class GLOW_API Texture : public Object
{
public:
    Texture(GLenum target = GL_TEXTURE_2D);
    Texture(GLuint id, GLenum target, bool ownsGLObject = true);

    static Texture * createDefault(GLenum target = GL_TEXTURE_2D);

    virtual void accept(ObjectVisitor & visitor) override;

    void bind() const;
    void unbind() const;
    static void unbind(GLenum target);

    void bindActive(GLenum texture) const;
    void unbindActive(GLenum texture) const;

    void setParameter(GLenum name, GLint value);
    void setParameter(GLenum name, GLfloat value);

    GLint getParameter(GLenum pname) const;
    GLint getLevelParameter(GLint level, GLenum pname) const;

    void getImage(GLint level, GLenum format, GLenum type, GLvoid * image) const;
    std::vector<unsigned char> getImage(GLint level, GLenum format, GLenum type) const;

    void getCompressedImage(GLint lod, GLvoid * image) const;
    std::vector<unsigned char> getCompressedImage(GLint lod = 0) const;

    GLenum target() const;

    void image1D(GLint level, GLenum internalFormat, GLsizei width, GLint border, GLenum format, GLenum type, const GLvoid * data);
    void compressedImage1D(GLint level, GLenum internalFormat, GLsizei width, GLint border, GLsizei imageSize, const GLvoid * data);
    void subImage1D(GLint level, GLint xOffset, GLsizei width, GLenum format, GLenum type, const GLvoid * data);

    void image2D(GLint level, GLenum internalFormat, GLsizei width, GLsizei height, GLint border, GLenum format, GLenum type, const GLvoid * data);
    void image2D(GLint level, GLenum internalFormat, const glm::ivec2 & size, GLint border, GLenum format, GLenum type, const GLvoid * data);
    void image2D(GLenum target, GLint level, GLenum internalFormat, GLsizei width, GLsizei height, GLint border, GLenum format, GLenum type, const GLvoid * data);
    void image2D(GLenum target, GLint level, GLenum internalFormat, const glm::ivec2 & size, GLint border, GLenum format, GLenum type, const GLvoid * data);
    void compressedImage2D(GLint level, GLenum internalFormat, GLsizei width, GLsizei height, GLint border, GLsizei imageSize, const GLvoid * data);
    void compressedImage2D(GLint level, GLenum internalFormat, const glm::ivec2 & size, GLint border, GLsizei imageSize, const GLvoid * data);
    void subImage2D(GLint level, GLint xOffset, GLint yOffset, GLsizei width, GLsizei height, GLenum format, GLenum type, const GLvoid * data);
    void subImage2D(GLint level, const glm::ivec2& offset, const glm::ivec2& size, GLenum format, GLenum type, const GLvoid * data);

    void image3D(GLint level, GLenum internalFormat, GLsizei width, GLsizei height, GLsizei depth, GLint border, GLenum format, GLenum type, const GLvoid * data);
    void image3D(GLint level, GLenum internalFormat, const glm::ivec3 & size, GLint border, GLenum format, GLenum type, const GLvoid * data);
    void compressedImage3D(GLint level, GLenum internalFormat, GLsizei width, GLsizei height, GLsizei depth, GLint border, GLsizei imageSize, const GLvoid * data);
    void compressedImage3D(GLint level, GLenum internalFormat, const glm::ivec3 & size, GLint border, GLsizei imageSize, const GLvoid * data);
    void subImage3D(GLint level, GLint xOffset, GLint yOffset, GLint zOffset, GLsizei width, GLsizei height, GLsizei depth, GLenum format, GLenum type, const GLvoid * data);
    void subImage3D(GLint level, const glm::ivec3& offset, const glm::ivec3& size, GLenum format, GLenum type, const GLvoid * data);

    void image2DMultisample(GLsizei samples, GLenum internalFormat, GLsizei width, GLsizei height, GLboolean fixedSamplesLocations);
    void image2DMultisample(GLsizei samples, GLenum internalFormat, const glm::ivec2 & size, GLboolean fixedSamplesLocations);
    void image3DMultisample(GLsizei samples, GLenum internalFormat, GLsizei width, GLsizei height, GLsizei depth, GLboolean fixedSamplesLocations);
    void image3DMultisample(GLsizei samples, GLenum internalFormat, const glm::ivec3 & size, GLboolean fixedSamplesLocations);

    void storage1D(GLsizei levels, GLenum internalFormat, GLsizei width);
    void storage2D(GLsizei levels, GLenum internalFormat, GLsizei width, GLsizei height);
    void storage2D(GLsizei levels, GLenum internalFormat, const glm::ivec2 & size);
    void storage3D(GLsizei levels, GLenum internalFormat, GLsizei width, GLsizei height, GLsizei depth);
    void storage3D(GLsizei levels, GLenum internalFormat, const glm::ivec3 & size);

    void textureView(GLuint originalTexture, GLenum internalFormat, GLuint minLevel, GLuint numLevels, GLuint minLayer, GLuint numLayers);

    void texBuffer(GLenum internalFormat, Buffer * buffer);
    void texBuffer(GLenum activeTexture, GLenum internalFormat, Buffer * buffer);
    void texBufferRange(GLenum internalFormat, Buffer * buffer, GLintptr offset, GLsizeiptr size);
    void texBufferRange(GLenum activeTexture, GLenum internalFormat, Buffer * buffer, GLintptr offset, GLsizeiptr size);

    void clearImage(GLint level, GLenum format, GLenum type, const void * data);
    void clearImage(GLint level, GLenum format, GLenum type, const glm::vec4 & value);
    void clearImage(GLint level, GLenum format, GLenum type, const glm::ivec4 & value);
    void clearImage(GLint level, GLenum format, GLenum type, const glm::uvec4 & value);

    void clearSubImage(GLint level, GLint xOffset, GLint yOffset, GLint zOffset, GLsizei width, GLsizei height, GLsizei depth, GLenum format, GLenum type, const void * data);
    void clearSubImage(GLint level, const glm::ivec3 & offset, const glm::ivec3 & size, GLenum format, GLenum type, const void * data);
    void clearSubImage(GLint level, const glm::ivec3 & offset, const glm::ivec3 & size, GLenum format, GLenum type, const glm::vec4 & value);
    void clearSubImage(GLint level, const glm::ivec3 & offset, const glm::ivec3 & size, GLenum format, GLenum type, const glm::ivec4 & value);
    void clearSubImage(GLint level, const glm::ivec3 & offset, const glm::ivec3 & size, GLenum format, GLenum type, const glm::uvec4 & value);

    void bindImageTexture(GLuint unit, GLint level, GLboolean layered, GLint layer, GLenum access, GLenum format) const;

    void generateMipmap();

    TextureHandle textureHandle() const;
    GLboolean isResident() const;
    TextureHandle makeResident() const;
    void makeNonResident() const;

    void pageCommitment(GLint level, GLint xOffset, GLint yOffset, GLint zOffset, GLsizei width, GLsizei height, GLsizei depth, GLboolean commit) const;
    void pageCommitment(GLint level, const glm::ivec3& offset, const glm::ivec3& size, GLboolean commit) const;

protected:
    virtual ~Texture();

    static GLuint genTexture();

protected:
    GLenum m_target;
};

} // namespace glow
