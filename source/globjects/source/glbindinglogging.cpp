
#include <globjects/glbindinglogging.h>

#include <sstream>
#include <iostream>

#include <glbinding/Version.h>
#include <glbinding-aux/Meta.h>
#include <glbinding/Value.h>
#include <glbinding-aux/types_to_string.h>


namespace globjects 
{


LogMessageBuilder operator<<(LogMessageBuilder builder, const glbinding::Version & version)
{
    builder << "Version " << version.toString();

    return builder;
}

LogMessageBuilder operator<<(LogMessageBuilder builder, const gl::GLboolean & value)
{
    builder << glbinding::aux::Meta::getString(value);

    return builder;
}

LogMessageBuilder operator<<(LogMessageBuilder builder, const gl::GLenum & value)
{
    builder << glbinding::aux::Meta::getString(value);

    return builder;
}

LogMessageBuilder operator<<(LogMessageBuilder builder, const gl::GLextension & value)
{
    builder << glbinding::aux::Meta::getString(value);

    return builder;
}

LogMessageBuilder operator<<(LogMessageBuilder builder, const gl::AttribMask & value)
{
    std::ostringstream stream;

    stream << value;

    builder << stream.str();

    return builder;
}

LogMessageBuilder operator<<(LogMessageBuilder builder, const gl::ClearBufferMask & value)
{
    std::ostringstream stream;

    stream << value;

    builder << stream.str();

    return builder;
}

LogMessageBuilder operator<<(LogMessageBuilder builder, const gl::ClientAttribMask & value)
{
    std::ostringstream stream;

    stream << value;

    builder << stream.str();

    return builder;
}

LogMessageBuilder operator<<(LogMessageBuilder builder, const gl::ContextFlagMask & value)
{
    std::ostringstream stream;

    stream << value;

    builder << stream.str();

    return builder;
}

LogMessageBuilder operator<<(LogMessageBuilder builder, const gl::ContextProfileMask & value)
{
    std::ostringstream stream;

    stream << value;

    builder << stream.str();

    return builder;
}

LogMessageBuilder operator<<(LogMessageBuilder builder, const gl::FfdMaskSGIX & value)
{
    std::ostringstream stream;

    stream << value;

    builder << stream.str();

    return builder;
}

LogMessageBuilder operator<<(LogMessageBuilder builder, const gl::FragmentShaderColorModMaskATI & value)
{
    std::ostringstream stream;

    stream << value;

    builder << stream.str();

    return builder;
}

LogMessageBuilder operator<<(LogMessageBuilder builder, const gl::FragmentShaderDestMaskATI & value)
{
    std::ostringstream stream;

    stream << value;

    builder << stream.str();

    return builder;
}

LogMessageBuilder operator<<(LogMessageBuilder builder, const gl::FragmentShaderDestModMaskATI & value)
{
    std::ostringstream stream;

    stream << value;

    builder << stream.str();

    return builder;
}

LogMessageBuilder operator<<(LogMessageBuilder builder, const gl::MapBufferUsageMask & value)
{
    std::ostringstream stream;

    stream << value;

    builder << stream.str();

    return builder;
}

LogMessageBuilder operator<<(LogMessageBuilder builder, const gl::MemoryBarrierMask & value)
{
    std::ostringstream stream;

    stream << value;

    builder << stream.str();

    return builder;
}

LogMessageBuilder operator<<(LogMessageBuilder builder, const gl::PathRenderingMaskNV & value)
{
    std::ostringstream stream;

    stream << value;

    builder << stream.str();

    return builder;
}

LogMessageBuilder operator<<(LogMessageBuilder builder, const gl::PerformanceQueryCapsMaskINTEL & value)
{
    std::ostringstream stream;

    stream << value;

    builder << stream.str();

    return builder;
}

LogMessageBuilder operator<<(LogMessageBuilder builder, const gl::SyncObjectMask & value)
{
    std::ostringstream stream;

    stream << value;

    builder << stream.str();

    return builder;
}

LogMessageBuilder operator<<(LogMessageBuilder builder, const gl::TextureStorageMaskAMD & value)
{
    std::ostringstream stream;

    stream << value;

    builder << stream.str();

    return builder;
}

LogMessageBuilder operator<<(LogMessageBuilder builder, const gl::UseProgramStageMask & value)
{
    std::ostringstream stream;

    stream << value;

    builder << stream.str();

    return builder;
}

LogMessageBuilder operator<<(LogMessageBuilder builder, const gl::VertexHintsMaskPGI & value)
{
    std::ostringstream stream;

    stream << value;

    builder << stream.str();

    return builder;
}

LogMessageBuilder operator<<(LogMessageBuilder builder, const gl::PathFontStyle & value)
{
    std::ostringstream stream;

    stream << value;

    builder << stream.str();

    return builder;
}

LogMessageBuilder operator<<(LogMessageBuilder builder, const gl::UnusedMask & value)
{
    std::ostringstream stream;

    stream << value;

    builder << stream.str();

    return builder;
}

LogMessageBuilder operator<<(LogMessageBuilder builder, const gl::BufferAccessMask & value)
{
    std::ostringstream stream;

    stream << value;

    builder << stream.str();

    return builder;
}


} // namespace globjects
