#pragma once

#include <set>
#include <string>

#include <glbinding/Extension.h>

#include <glow/glow_api.h>

namespace glow
{

class Version;

class GLOW_API ExtensionRegistry
{
public:
    ExtensionRegistry();
    static ExtensionRegistry & current();

    std::set<gl::Extension>::iterator begin();
    std::set<gl::Extension>::iterator end();

    const std::set<gl::Extension> & availableExtensions();
    const std::set<std::string> & unknownAvailableExtensions();

    bool hasExtension(gl::Extension extension);
    bool hasExtension(const std::string & extensionName);
    bool isInCoreProfile(gl::Extension extension);
    bool isInCoreProfile(gl::Extension extension, const Version & version);
protected:
    bool m_initialized;
    std::set<gl::Extension> m_availableExtensions;
    std::set<std::string> m_unknownAvailableExtensions;

    void initialize();
};

} // namespace glow
