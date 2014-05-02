#include <glow/Extension.h>

#include <set>

#include <glow/glow.h>
#include <glow/gl_extension_info.h>

namespace glow {

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
