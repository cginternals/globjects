#include <glow/Capability.h>

#include <glow/State.h>
#include <glow/Error.h>
#include <glow/glow.h>

namespace glow {

Capability::Capability(gl::GLenum capability)
: m_capability(capability)
, m_enabled(false)
{
}

Capability::Capability(gl::GLenum capability, bool enabled)
: m_capability(capability)
, m_enabled(enabled)
{
}

gl::GLenum Capability::capability() const
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

void Capability::enable(int index)
{
    m_indexEnabled[index] = true;
}

void Capability::disable(int index)
{
    m_indexEnabled[index] = false;
}

bool Capability::isEnabled(int index) const
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

} // namespace glow
