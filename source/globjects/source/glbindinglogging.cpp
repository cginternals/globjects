
#include <globjects/glbindinglogging.h>

#include <glbinding/Version.h>


namespace glo 
{

LogMessageBuilder operator<<(LogMessageBuilder builder, const glbinding::Version & version)
{
    builder << "Version " << version.toString();

    return builder;
}

} // namespace glo
