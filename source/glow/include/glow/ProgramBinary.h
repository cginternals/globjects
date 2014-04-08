#pragma once

#include <GL/glew.h>

#include <vector>

#include <glow/glow.h>
#include <glow/Referenced.h>
#include <glow/ChangeListener.h>
#include <glow/Changeable.h>
#include <glow/ref_ptr.h>

namespace glow
{

class AbstractStringSource;

/**
 * \brief The ProgramBinary class is used for directly setting binary sources for a Program.
 *
 * \see Program
 * \see http://www.opengl.org/registry/specs/ARB/get_program_binary.txt
 */
class GLOW_API ProgramBinary : public Referenced, public Changeable, protected ChangeListener
{
public:
    ProgramBinary(GLenum binaryFormat, const std::vector<char> & binaryData);
    ProgramBinary(GLenum binaryFormat, AbstractStringSource * dataSource);
    virtual ~ProgramBinary();

    GLenum format() const;
    const void * data() const;
    GLsizei length() const;

     virtual void notifyChanged(const Changeable* sender) override;
protected:
    GLenum m_binaryFormat;
    ref_ptr<AbstractStringSource> m_dataSource;

    mutable bool m_valid;
    mutable std::vector<unsigned char> m_binaryData;

    void validate() const;
};

} // namespace glow
