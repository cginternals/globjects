#pragma once

#include <glow/glow.h>
#include <glow/Object.h>
#include <glow/TextureHandle.h>

#include <glm/glm.hpp>

namespace glow 
{

/** \brief Wraps OpenGL texture objects.
    
    A Texture provides both interfaces to bind them for the OpenGL pipeline:
    binding and bindless texture. Bindless textures are only available if the
    graphics driver supports them (NV extension).
    
    \see http://www.opengl.org/wiki/Texture
    \see http://www.opengl.org/registry/specs/NV/bindless_texture.txt
 */
class GLOW_API Texture : public Object
{
public:
	Texture(GLenum  target = GL_TEXTURE_2D);
	Texture(GLuint id, GLenum  target, bool ownsGLObject = true);
	virtual ~Texture();

	virtual void accept(ObjectVisitor & visitor);

	void bind() const;
    void unbind() const;
    static void unbind(GLenum target);

    void bindActive(GLenum texture) const;
    void unbindActive(GLenum texture) const;

	void setParameter(GLenum name, GLint value);
	void setParameter(GLenum name, GLfloat value);

	GLint getParameter(GLenum pname);
	GLint getLevelParameter(GLint level, GLenum pname);

	GLenum target() const;

    void image1D(
        GLint level
    ,   GLint internalFormat
    ,   GLsizei width
    ,   GLint border
    ,   GLenum format
    ,   GLenum type
    ,   const GLvoid * data);

	void image2D(
        GLint level
    ,   GLint internalFormat
    ,   GLsizei width
    ,   GLsizei height
    ,   GLint border
    ,   GLenum format
    ,   GLenum type
    ,   const GLvoid * data);

	void image3D(
        GLint level
    ,   GLint internalFormat
    ,   GLsizei width
    ,   GLsizei height
    ,   GLsizei depth
    ,   GLint border
    ,   GLenum format
    ,   GLenum type
    ,   const GLvoid * data);

    void storage1D(
        GLsizei levels
    ,   GLenum internalFormat
    ,   GLsizei width);

    void storage2D(
        GLsizei levels
    ,   GLenum internalFormat
    ,   GLsizei width
    ,   GLsizei height);

    void storage3D(
        GLsizei levels
    ,   GLenum internalFormat
    ,   GLsizei width
    ,   GLsizei height
    ,   GLsizei depth);

    void clearImage(
        GLint level
    ,   GLenum format
    ,   GLenum type
    ,   const void * data);

    void clearSubImage(
        GLint level
    ,   GLint xOffset
    ,   GLint yOffset
    ,   GLint zOffset
    ,   GLsizei width
    ,   GLsizei height
    ,   GLsizei depth
    ,   GLenum format
    ,   GLenum type
    ,   const void * data);

    void clearSubImage(
        GLint level
    ,   const glm::ivec3 & offset
    ,   const glm::ivec3 & size
    ,   GLenum format
    ,   GLenum type
    ,   const void * data);

	void bindImageTexture(
        GLuint unit
    ,   GLint level
    ,   GLboolean layered
    ,   GLint layer
    ,   GLenum access
    ,   GLenum format);

	void generateMipmap();

    TextureHandle textureHandle() const;

	GLboolean isResident() const;
    TextureHandle makeResident();
	void makeNonResident();

protected:
    static GLuint genTexture();

protected:
    GLenum m_target;
};

} // namespace glow
