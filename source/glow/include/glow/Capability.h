#pragma once

#include <map>

#include <GL/glew.h>

#include <glow/glow_api.h>

namespace glow
{

class GLOW_API Capability
{
public:
    Capability(GLenum capability);
    Capability(GLenum capability, bool enabled);

    GLenum capability() const;

    void enable();
    void disable();
    bool isEnabled() const;

    void enable(int index);
    void disable(int index);
    bool isEnabled(int index) const;

    void apply();
protected:
    GLenum m_capability;
    bool m_enabled;
    std::map<int, bool> m_indexEnabled;
};

} // namespace glow
