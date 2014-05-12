#include "ExtensionRegistry.h"
#include "Registry.h"

#include <glbinding/meta.h>

#include <glow/glow.h>

namespace glow {

ExtensionRegistry::ExtensionRegistry()
: m_initialized(false)
{
}

ExtensionRegistry & ExtensionRegistry::current()
{
    return Registry::current().extensions();
}


std::set<gl::Extension>::iterator ExtensionRegistry::begin()
{
    return availableExtensions().begin();
}

std::set<gl::Extension>::iterator ExtensionRegistry::end()
{
    return availableExtensions().end();
}

const std::set<gl::Extension> & ExtensionRegistry::availableExtensions()
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
        gl::Extension extension = gl::meta::extensionFromString(extensionName);

        if (extension != gl::Extension::Unknown)
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

bool ExtensionRegistry::hasExtension(gl::Extension extension)
{
    initialize();

    if (isInCoreProfile(extension))
        return true;

    return m_availableExtensions.find(extension) != m_availableExtensions.end();
}

bool ExtensionRegistry::hasExtension(const std::string & extensionName)
{
    initialize();

    gl::Extension extension = gl::meta::extensionFromString(extensionName);

    if (extension != gl::Extension::Unknown)
    {
        return hasExtension(extension);
    }
    else
    {
        return m_unknownAvailableExtensions.find(extensionName) != m_unknownAvailableExtensions.end();
    }
}

Version getCoreVersion(gl::Extension extension)
{
    auto pair = gl::meta::coreVersionForExtension(extension);
    if (pair.first > 0)
    {
        return Version(pair.first, pair.second);
    }

    return Version();
}

bool ExtensionRegistry::isInCoreProfile(gl::Extension extension, const Version & version)
{
    Version coreVersion = getCoreVersion(extension);

    if (!coreVersion.isValid())
        return false;

    return coreVersion <= version;
}


bool ExtensionRegistry::isInCoreProfile(gl::Extension extension)
{
    return isInCoreProfile(extension, glow::version());
}

} // namespace glow

