#include "NamedStringRegistry.h"
#include "Registry.h"

#include <glow/logging.h>
#include <glow/NamedString.h>

#include <glow/glow.h>

namespace glow {

NamedStringRegistry::NamedStringRegistry()
{
}

NamedStringRegistry & NamedStringRegistry::current()
{
    return Registry::current().namedStrings();
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
        warning() << "Registering NamedString with existing name " << namedString->name();
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
    return hasExtension(gl::Extension::ARB_shading_language_include);
}

} // namespace glow
