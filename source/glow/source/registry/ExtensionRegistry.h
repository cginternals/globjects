#pragma once

#include <set>
#include <string>

#include <glow/glow_api.h>
#include <glow/Extension.h>

namespace glow
{

class Version;

class GLOW_API ExtensionRegistry
{
public:
    ExtensionRegistry();
    static ExtensionRegistry & current();

    std::set<glow::Extension>::iterator begin();
    std::set<glow::Extension>::iterator end();

    const std::set<glow::Extension> & availableExtensions();
    const std::set<std::string> & unknownAvailableExtensions();

    bool hasExtension(Extension extension);
    bool hasExtension(const std::string & extensionName);
    bool isInCoreProfile(Extension extension);
    bool isInCoreProfile(Extension extension, const Version & version);
protected:
    bool m_initialized;
    std::set<glow::Extension> m_availableExtensions;
    std::set<std::string> m_unknownAvailableExtensions;

    void initialize();
};

} // namespace glow
