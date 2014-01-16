#include "FeatureRegistry.h"

#include <glow/Version.h>

#include "gl_extensions.h"

namespace glow {

FeatureRegistry FeatureRegistry::s_instance;

FeatureRegistry& FeatureRegistry::instance()
{
    return s_instance;
}

bool FeatureRegistry::supportsFeature(Extension extension) const
{
    return !glow::Version::currentVersionIsInCoreProfile() && m_availableExtensions.find(extension) != m_availableExtensions.end();
}

void FeatureRegistry::addSupportedFeature(Extension extension)
{
    m_availableExtensions.insert(extension);
}

void FeatureRegistry::addSupportedFeature(const std::string& extensionName)
{
    addSupportedFeature(getExtensionValue(extensionName));
}

Extension FeatureRegistry::getExtensionValue(const std::string& extensionName)
{
    std::unordered_map<std::string, glow::Extension>::const_iterator it = GLextensions.find(extensionName);

    return it == GLextensions.end() ? GLOW_Unknown : it->second;
}

FeatureRegistry::FeatureRegistry()
{
}

} // namespace glow
