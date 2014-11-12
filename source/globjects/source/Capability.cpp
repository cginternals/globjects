
#include <globjects/Capability.h>

#include <glbinding/gl/enum.h>

#include <globjects/State.h>
#include <globjects/globjects.h>


using namespace gl;

namespace globjects 
{

Capability::Capability(const GLenum capability)
: m_capability(capability)
, m_enabled(false)
{
}

Capability::Capability(const GLenum capability, const bool enabled)
: m_capability(capability)
, m_enabled(enabled)
{
}

GLenum Capability::capability() const
{
    return m_capability;
}

void Capability::enable()
{
    m_enabled = true;
}

void Capability::disable()
{
    m_enabled = false;
}

bool Capability::isEnabled() const
{
    return m_enabled;
}

void Capability::enable(const int index)
{
    m_indexEnabled[index] = true;
}

void Capability::disable(const int index)
{
    m_indexEnabled[index] = false;
}

bool Capability::isEnabled(const int index) const
{
    return m_indexEnabled.at(index);
}

void Capability::apply()
{
    if (m_indexEnabled.empty())
    {
        setEnabled(m_capability, m_enabled);
    }
    else
    {
        for (const std::pair<int, bool>& pair : m_indexEnabled)
        {
            setEnabled(m_capability, pair.first, pair.second);
        }
    }
}

} // namespace globjects
