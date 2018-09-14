
#include <globjects/ProgramBinary.h>

#include <cassert>

#include <globjects/Program.h>


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
    while (!m_programListeners.empty())
    {
        // calls deregisterListener
        (*m_programListeners.begin())->removeSubject(this);
    }
}

void ProgramBinary::changed() const
{
    for (Program * listener: m_programListeners)
    {
        listener->notifyChanged(this);
    }
}

void ProgramBinary::registerListener(Program * listener)
{
    assert(listener != nullptr);

    m_programListeners.insert(listener);
    listener->addSubject(this);
}

void ProgramBinary::deregisterListener(Program * listener)
{
    assert(listener != nullptr);

    const auto it = m_programListeners.find(listener);

    if (it == m_programListeners.end())
    {
        return;
    }

    m_programListeners.erase(it);
    listener->removeSubject(this);
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
