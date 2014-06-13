#include <glow/ProgramBinary.h>

#include <glowbase/StaticStringSource.h>
#include <glowbase/AbstractStringSource.h>

namespace glow
{

ProgramBinary::ProgramBinary(gl::GLenum binaryFormat, const std::vector<char> & binaryData)
: ProgramBinary(binaryFormat, new StaticStringSource(binaryData.data(), binaryData.size()))
{
}

ProgramBinary::ProgramBinary(gl::GLenum binaryFormat, AbstractStringSource * dataSource)
: m_binaryFormat(binaryFormat)
, m_dataSource(dataSource)
, m_valid(false)
{
    if (m_dataSource)
    {
        m_dataSource->registerListener(this);
    }
}

ProgramBinary::~ProgramBinary()
{
    if (m_dataSource)
    {
        m_dataSource->deregisterListener(this);
    }
}

gl::GLenum ProgramBinary::format() const
{
    return m_binaryFormat;
}

const void * ProgramBinary::data() const
{
    validate();

    return reinterpret_cast<const void*>(m_binaryData.data());
}

gl::GLsizei ProgramBinary::length() const
{
    validate();

    return static_cast<gl::GLsizei>(m_binaryData.size());
}

void ProgramBinary::notifyChanged(const Changeable *)
{
    m_valid = false;
    changed();
}

void ProgramBinary::validate() const
{
    if (m_valid)
        return;

    if (!m_dataSource)
        return;

    std::string stringData = m_dataSource->string();
    size_t length = stringData.size();
    const unsigned char* data = reinterpret_cast<const unsigned char*>(stringData.c_str());

    m_binaryData = std::vector<unsigned char>(data, data+length);

    m_valid = true;
}

} // namespace glow
