#include <glow/objectlogging.h>

#include <glow/Object.h>
#include <glow/Buffer.h>
#include <glow/FrameBufferObject.h>
#include <glow/Program.h>
#include <glow/Query.h>
#include <glow/RenderBufferObject.h>
#include <glow/Sampler.h>
#include <glow/Shader.h>
#include <glow/Sync.h>
#include <glow/Texture.h>
#include <glow/TransformFeedback.h>
#include <glow/VertexArrayObject.h>
#include <glow/AbstractUniform.h>
#include <glow/Version.h>

namespace glow {

void logObject(glowbase::LogMessageBuilder & builder, const Object * object, const std::string & typeName)
{
    builder << typeName << "(" << object->id();
    if (object->hasName())
        builder << ", " << object->name();
    builder << ")";
}


glowbase::LogMessageBuilder operator<<(glowbase::LogMessageBuilder builder, const Object * object)
{
    logObject(builder, object, "Object");
    return builder;
}

glowbase::LogMessageBuilder operator<<(glowbase::LogMessageBuilder builder, const Buffer * object)
{
    logObject(builder, object, "Buffer");
    return builder;
}

glowbase::LogMessageBuilder operator<<(glowbase::LogMessageBuilder builder, const FrameBufferObject * object)
{
    logObject(builder, object, "FrameBufferObject");
    return builder;
}

glowbase::LogMessageBuilder operator<<(glowbase::LogMessageBuilder builder, const Program * object)
{
    logObject(builder, object, "Program");
    return builder;
}

glowbase::LogMessageBuilder operator<<(glowbase::LogMessageBuilder builder, const Query * object)
{
    logObject(builder, object, "Query");
    return builder;
}

glowbase::LogMessageBuilder operator<<(glowbase::LogMessageBuilder builder, const RenderBufferObject * object)
{
    logObject(builder, object, "RenderBufferObject");
    return builder;
}

glowbase::LogMessageBuilder operator<<(glowbase::LogMessageBuilder builder, const Sampler * object)
{
    logObject(builder, object, "Sampler");
    return builder;
}

glowbase::LogMessageBuilder operator<<(glowbase::LogMessageBuilder builder, const Shader * object)
{
    logObject(builder, object, "Shader");
    return builder;
}

glowbase::LogMessageBuilder operator<<(glowbase::LogMessageBuilder builder, const Texture * object)
{
    logObject(builder, object, "Texture");
    return builder;
}

glowbase::LogMessageBuilder operator<<(glowbase::LogMessageBuilder builder, const TransformFeedback * object)
{
    logObject(builder, object, "TransformFeedback");
    return builder;
}

glowbase::LogMessageBuilder operator<<(glowbase::LogMessageBuilder builder, const VertexArrayObject * object)
{
    logObject(builder, object, "VertexArrayObject");
    return builder;
}

glowbase::LogMessageBuilder operator<<(glowbase::LogMessageBuilder builder, const Sync * sync)
{
    builder << "Sync(" << sync->sync() << ")";

    return builder;
}

glowbase::LogMessageBuilder operator<<(glowbase::LogMessageBuilder builder, const AbstractUniform * uniform)
{
    builder << "AbstractUniform" << "(";
    if (uniform->identity().isName())
        builder << uniform->identity().name();
    else
        builder << uniform->identity().location();
    builder << ")";

    return builder;
}

glowbase::LogMessageBuilder operator<<(glowbase::LogMessageBuilder builder, const Version & version)
{
    builder << "Version " << version.toString();

    return builder;
}

} // namespace glow
