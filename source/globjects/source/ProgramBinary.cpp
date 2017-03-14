
#include <globjects/ProgramBinary.h>

#include <globjects/base/StaticStringSource.h>
#include <globjects/base/AbstractStringSource.h>

using namespace gl;


namespace globjects
{


ProgramBinary::ProgramBinary(const GLenum binaryFormat, const std::vector<unsigned char> & data)
: m_binaryFormat(binaryFormat)
, m_binaryData(data)
{
}

ProgramBinary::~ProgramBinary()
{
}

GLenum ProgramBinary::format() const
{
    return m_binaryFormat;
}

const void * ProgramBinary::data() const
{
    return reinterpret_cast<const void*>(m_binaryData.data());
}

GLsizei ProgramBinary::length() const
{
    return static_cast<GLsizei>(m_binaryData.size());
}


} // namespace globjects
