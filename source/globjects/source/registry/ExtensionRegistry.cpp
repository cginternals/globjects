#include "ExtensionRegistry.h"
#include "Registry.h"

#include <glbinding/Meta.h>
#include <glbinding/ContextInfo.h>
#include <glbinding/Version.h>

#include <globjects/globjects.h>

using namespace gl;

namespace globjects 
{

ExtensionRegistry::ExtensionRegistry()
: m_initialized(false)
{
}

ExtensionRegistry & ExtensionRegistry::current()
{
    return Registry::current().extensions();
}


std::set<GLextension>::iterator ExtensionRegistry::begin()
{
    return availableExtensions().begin();
}

std::set<GLextension>::iterator ExtensionRegistry::end()
{
    return availableExtensions().end();
}

const std::set<GLextension> & ExtensionRegistry::availableExtensions()
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

    m_availableExtensions = glbinding::ContextInfo::extensions(&m_unknownAvailableExtensions);

    m_initialized = true;
}

bool ExtensionRegistry::hasExtension(GLextension extension)
{
    initialize();

    if (isInCoreProfile(extension))
        return true;

    return m_availableExtensions.find(extension) != m_availableExtensions.end();
}

bool ExtensionRegistry::hasExtension(const std::string & extensionName)
{
    initialize();

    GLextension extension = glbinding::Meta::getExtension(extensionName);

    if (extension != GLextension::UNKNOWN)
    {
        return hasExtension(extension);
    }
    else
    {
        return m_unknownAvailableExtensions.find(extensionName) != m_unknownAvailableExtensions.end();
    }
}

glbinding::Version getCoreVersion(GLextension extension)
{
    return glbinding::Meta::getRequiringVersion(extension);
}

bool ExtensionRegistry::isInCoreProfile(GLextension extension, const glbinding::Version & version)
{
    glbinding::Version coreVersion = getCoreVersion(extension);

    if (!coreVersion.isValid())
        return false;

    return coreVersion <= version;
}


bool ExtensionRegistry::isInCoreProfile(GLextension extension)
{
    return isInCoreProfile(extension, globjects::version());
}

} // namespace globjects

