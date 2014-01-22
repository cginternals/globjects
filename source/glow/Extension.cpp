#include <set>

#include <glow/global.h>
#include <glow/Extension.h>

#include <gl_extension_info.h>


namespace {

std::set<glow::Extension> availableExtensions;
std::set<std::string> additionalAvailableExtensions;

bool extensionInitialized = false;

void initializeExtensions()
{
    if (extensionInitialized)
        return;

    for (const std::string & extensionName : glow::getExtensions())
    {
        glow::Extension extension = glow::extensionFromString(extensionName);

        if (extension != glow::GLOW_Unknown_Extension)
        {
            availableExtensions.insert(extension);
        }
        else
        {
            additionalAvailableExtensions.insert(extensionName);
        }
    }

    extensionInitialized = true;
}

}

namespace glow {

bool hasExtension(Extension extension)
{
    initializeExtensions();

    auto it = extensionVersions.find(extension);
    if (it != extensionVersions.end() && it->second <= Version::current())
    {
        return true;
    }

    return availableExtensions.find(extension) != availableExtensions.end();
}

bool hasExtension(const std::string & extensionName)
{
    initializeExtensions();

    Extension extension = extensionFromString(extensionName);

    if (extension != glow::GLOW_Unknown_Extension)
    {
        return hasExtension(extension);
    }
    else
    {
        return additionalAvailableExtensions.find(extensionName) != additionalAvailableExtensions.end();
    }
}

std::string extensionString(Extension extension)
{
    auto it = extensionStrings.find(extension);
    if (it == extensionStrings.end())
        return "UnknownExtension";

    return it->second;
}

Extension extensionFromString(const std::string & extensionName)
{
    auto it = extensions.find(extensionName);
    if (it == extensions.end())
        return GLOW_Unknown_Extension;

    return it->second;
}


} // namespace glow
