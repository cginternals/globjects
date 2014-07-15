
#include <glow/glbindinglogging.h>

#include <glbinding/Version.h>


namespace glow 
{

LogMessageBuilder operator<<(LogMessageBuilder builder, const glbinding::Version & version)
{
    builder << "Version " << version.toString();

    return builder;
}

} // namespace glow
