
#pragma once

#include <vector>

#include <glbinding/gl/types.h>

#include <globjects/globjects_api.h>

#include <globjects/base/Referenced.h>
#include <globjects/base/ChangeListener.h>
#include <globjects/base/Changeable.h>
#include <globjects/base/ref_ptr.h>

namespace globjects
{


class AbstractStringSource;

/**
 * \brief The ProgramBinary class is used for directly setting binary sources for a Program.
 *
 * \see Program
 * \see http://www.opengl.org/registry/specs/ARB/get_program_binary.txt
 */
class GLOBJECTS_API ProgramBinary : public Referenced, public Changeable, protected ChangeListener
{
public:
    ProgramBinary(gl::GLenum binaryFormat, const std::vector<char> & binaryData);
    ProgramBinary(gl::GLenum binaryFormat, AbstractStringSource * dataSource);

    gl::GLenum format() const;
    const void * data() const;
    gl::GLsizei length() const;

     virtual void notifyChanged(const Changeable* sender) override;

protected:
    virtual ~ProgramBinary();

    void validate() const;

protected:
    gl::GLenum m_binaryFormat;
    ref_ptr<AbstractStringSource> m_dataSource;

    mutable bool m_valid;
    mutable std::vector<unsigned char> m_binaryData;
};


} // namespace globjects
