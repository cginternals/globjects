#pragma once

#include <GL/glew.h>

#include <glowbase/ref_ptr.h>

#include <glow/glow_api.h>
#include <glow/FrameBufferAttachment.h>

namespace glow 
{

class Texture;
class FrameBufferObject;

/** \brief Encapsulates texture attachments of a frame buffer object.
    
    This class is a link between a Texture and the FrameBufferObject it is attached to.
    
    \see Texture
    \see FrameBufferObject
 */
class GLOW_API TextureAttachment : public FrameBufferAttachment
{
public:
    TextureAttachment(FrameBufferObject * fbo, GLenum attachment, Texture * texture, GLint level, GLint layer = -1);

    virtual bool isTextureAttachment() const override;
	Texture * texture();
    const Texture * texture() const;

    GLint level() const;

    bool hasLayer() const;
    GLint layer() const;
protected:
    ref_ptr<Texture> m_texture;
    GLint m_level;
    GLint m_layer;
};

} // namespace glow
