#pragma once

#include <GL/glew.h>

#include <glow/glow.h>
#include <glow/ref_ptr.h>
#include <glow/FrameBufferAttachment.h>

namespace glow 
{

class Texture;

/** \brief Encapsulates texture attachments of a frame buffer object.
    
    This class is a link between a Texture and the FrameBufferObject it is attached to.
    
    \see Texture
    \see FrameBufferObject
 */
class GLOW_API TextureAttachment : public FrameBufferAttachment
{
public:
    TextureAttachment(Texture * texture, GLenum attachment, GLint level, GLint layer = -1);

    virtual bool isTextureAttachment() const override;
	Texture * texture();

    GLint level() const;

    bool hasLayer() const;
    GLint layer() const;
protected:
	ref_ptr<Texture> m_texture;
    GLint m_level;
    GLint m_layer;
};

} // namespace glow
