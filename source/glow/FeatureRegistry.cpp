#include "FeatureRegistry.h"

#include <glow/Version.h>

#include <gl_extension_info.h>

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
    const auto it = extensions.find(extensionName);

    return it == extensions.cend() ? GLOW_Unknown_Extension : it->second;
}

FeatureRegistry::FeatureRegistry()
{
}

} // namespace glow
