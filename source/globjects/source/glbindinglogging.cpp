
#include <globjects/glbindinglogging.h>

#include <glbinding/Version.h>


namespace globjects 
{

LogMessageBuilder operator<<(LogMessageBuilder builder, const glbinding::Version & version)
{
    builder << "Version " << version.toString();

    return builder;
}

} // namespace globjects
