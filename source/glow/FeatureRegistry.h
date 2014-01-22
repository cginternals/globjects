#pragma once

#include <unordered_set>
#include <utility>
#include <string>

#include <glow/Extension.h>

namespace glow {

class FeatureRegistry
{
public:
    static FeatureRegistry& instance();

    bool supportsFeature(Extension extension) const;

    void addSupportedFeature(Extension extension);
    void addSupportedFeature(const std::string& extensionName);

    static Extension getExtensionValue(const std::string& extensionName);
protected:
    std::unordered_set<Extension, std::hash<unsigned int>> m_availableExtensions;
private:
    FeatureRegistry();

    static FeatureRegistry s_instance;
};

} // namespace glow
