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
	TextureAttachment(Texture * texture, GLenum attachment);

    virtual bool isTextureAttachment() const override;
	Texture * texture();

protected:
	ref_ptr<Texture> m_texture;
};

} // namespace glow
