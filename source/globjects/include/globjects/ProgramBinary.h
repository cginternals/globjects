
#pragma once

#include <vector>

#include <glbinding/gl/types.h>

#include <globjects/globjects_api.h>

#include <globjects/base/ChangeListener.h>
#include <globjects/base/Changeable.h>
#include <globjects/base/Instantiator.h>

namespace globjects
{


class AbstractStringSource;

/**
 * \brief The ProgramBinary class is used for directly setting binary sources for a Program.
 *
 * \see Program
 * \see http://www.opengl.org/registry/specs/ARB/get_program_binary.txt
 */
class GLOBJECTS_API ProgramBinary : public Changeable, public Instantiator<ProgramBinary>
{
public:
    ProgramBinary(gl::GLenum binaryFormat, const std::vector<unsigned char> & data);
    virtual ~ProgramBinary();

    gl::GLenum format() const;
    const void * data() const;
    gl::GLsizei length() const;

protected:
    gl::GLenum m_binaryFormat;
    std::vector<unsigned char> m_binaryData;
};


} // namespace globjects
