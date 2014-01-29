#pragma once

#include <GL/glew.h>

#include <vector>

#include <glow/glow.h>
#include <glow/Referenced.h>
#include <glow/ChangeListener.h>
#include <glow/AbstractStringSource.h>
#include <glow/ref_ptr.h>


namespace glow
{

class GLOW_API ProgramBinary : public Referenced, protected ChangeListener
{
public:
    ProgramBinary(GLenum binaryFormat, const std::vector<unsigned char> & binaryData);
    ProgramBinary(GLenum binaryFormat, AbstractStringSource * binaryData);

    GLenum format() const;
    void * data() const;
    GLsizei length() const;
protected:
    GLenum m_binaryFormat;
    ref_ptr<AbstractStringSource> m_binaryData;
};

} // namespace glow
