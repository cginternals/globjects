#include "NamedStringRegistry.h"

#include <glow/logging.h>

#include <glow/global.h>

namespace glow {

NamedStringRegistry::NamedStringRegistry()
{
}

bool NamedStringRegistry::hasNamedString(const std::string & name)
{
    return m_namedStrings.find(name) != m_namedStrings.end();
}

NamedString * NamedStringRegistry::namedString(const std::string & name)
{
    auto it = m_namedStrings.find(name);

    return it == m_namedStrings.end() ? nullptr : it->second;
}

void NamedStringRegistry::registerNamedString(NamedString * namedString)
{
    if (hasNamedString(namedString->name()))
    {
        glow::warning() << "Registering NamedString with existing name " << namedString->name();
    }

    m_namedStrings[namedString->name()] = namedString;
}

void NamedStringRegistry::deregisterNamedString(NamedString * namedString)
{
    m_namedStrings.erase(namedString->name());
}

bool NamedStringRegistry::hasNativeSupport()
{
    // TODO: cache
    return hasExtension(GLOW_ARB_shading_language_include);
}

} // namespace glow
