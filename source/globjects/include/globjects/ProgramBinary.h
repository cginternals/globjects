
#pragma once


#include <vector>
#include <set>

#include <glbinding/gl/types.h>

#include <globjects/globjects_api.h>

#include <globjects/base/Instantiator.h>


namespace globjects
{


class Program;


/**
 * \brief The ProgramBinary class is used for directly setting binary sources for a Program.
 *
 * \see Program
 * \see http://www.opengl.org/registry/specs/ARB/get_program_binary.txt
 */
class GLOBJECTS_API ProgramBinary : public Instantiator<ProgramBinary>
{
public:
    ProgramBinary(gl::GLenum binaryFormat, const std::vector<unsigned char> & data);
    virtual ~ProgramBinary();

    gl::GLenum format() const;
    const void * data() const;
    gl::GLsizei length() const;

    void changed() const;

    void registerListener(Program * listener);
    void deregisterListener(Program * listener);

protected:
    std::set<Program *> m_programListeners;
    gl::GLenum m_binaryFormat;
    std::vector<unsigned char> m_binaryData;
};


} // namespace globjects
