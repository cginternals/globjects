
#pragma once

#include <map>

#include <glbinding/gl/types.h>

#include <globjects/globjects_api.h>


namespace globjects
{


class GLOBJECTS_API Capability
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


} // namespace globjects
