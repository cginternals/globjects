#include "ExtensionRegistry.h"
#include "Registry.h"

#include <glow/glow.h>
#include <glow/gl_extension_info.h>

namespace glow {

ExtensionRegistry::ExtensionRegistry()
: m_initialized(false)
{
}

ExtensionRegistry & ExtensionRegistry::current()
{
    return Registry::current().extensions();
}


std::set<Extension>::iterator ExtensionRegistry::begin()
{
    return availableExtensions().begin();
}

std::set<Extension>::iterator ExtensionRegistry::end()
{
    return availableExtensions().end();
}

const std::set<Extension> & ExtensionRegistry::availableExtensions()
{
    initialize();
    return m_availableExtensions;
}

const std::set<std::string> & ExtensionRegistry::unknownAvailableExtensions()
{
    initialize();
    return m_unknownAvailableExtensions;
}

void ExtensionRegistry::initialize()
{
    if (m_initialized)
        return;

    for (const std::string & extensionName : getExtensions())
    {
        Extension extension = extensionFromString(extensionName);

        if (extension != GLOW_Unknown_Extension)
        {
            m_availableExtensions.insert(extension);
        }
        else
        {
            m_unknownAvailableExtensions.insert(extensionName);
        }
    }

    m_initialized = true;
}

bool ExtensionRegistry::hasExtension(Extension extension)
{
    initialize();

    if (isInCoreProfile(extension))
        return true;

    return m_availableExtensions.find(extension) != m_availableExtensions.end();
}

bool ExtensionRegistry::hasExtension(const std::string & extensionName)
{
    initialize();

    Extension extension = extensionFromString(extensionName);

    if (extension != GLOW_Unknown_Extension)
    {
        return hasExtension(extension);
    }
    else
    {
        return m_unknownAvailableExtensions.find(extensionName) != m_unknownAvailableExtensions.end();
    }
}

bool ExtensionRegistry::isInCoreProfile(Extension extension, const Version & version)
{
    auto it = extensionVersions.find(extension);
    if (it == extensionVersions.end())
        return false;

    return it->second <= version;
}


bool ExtensionRegistry::isInCoreProfile(Extension extension)
{
    return isInCoreProfile(extension, glow::version());
}

} // namespace glow

