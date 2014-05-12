#pragma once

#include <map>

#include <glbinding/types.h>

#include <glow/glow_api.h>

namespace glow
{

class GLOW_API Capability
{
public:
    Capability(gl::GLenum capability);
    Capability(gl::GLenum capability, bool enabled);

    gl::GLenum capability() const;

    void enable();
    void disable();
    bool isEnabled() const;

    void enable(int index);
    void disable(int index);
    bool isEnabled(int index) const;

    void apply();
protected:
    gl::GLenum m_capability;
    bool m_enabled;
    std::map<int, bool> m_indexEnabled;
};

} // namespace glow
