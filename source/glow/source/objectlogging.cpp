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

namespace glow {

void logObject(LogMessageBuilder & builder, const Object * object, const std::string & typeName)
{
    builder << typeName << "(" << object->id();
    if (object->hasName())
        builder << ", " << object->name();
    builder << ")";
}


LogMessageBuilder operator<<(LogMessageBuilder builder, const Object * object)
{
    logObject(builder, object, "Object");
    return builder;
}

LogMessageBuilder operator<<(LogMessageBuilder builder, const Buffer * object)
{
    logObject(builder, object, "Buffer");
    return builder;
}

LogMessageBuilder operator<<(LogMessageBuilder builder, const FrameBufferObject * object)
{
    logObject(builder, object, "FrameBufferObject");
    return builder;
}

LogMessageBuilder operator<<(LogMessageBuilder builder, const Program * object)
{
    logObject(builder, object, "Program");
    return builder;
}

LogMessageBuilder operator<<(LogMessageBuilder builder, const Query * object)
{
    logObject(builder, object, "Query");
    return builder;
}

LogMessageBuilder operator<<(LogMessageBuilder builder, const RenderBufferObject * object)
{
    logObject(builder, object, "RenderBufferObject");
    return builder;
}

LogMessageBuilder operator<<(LogMessageBuilder builder, const Sampler * object)
{
    logObject(builder, object, "Sampler");
    return builder;
}

LogMessageBuilder operator<<(LogMessageBuilder builder, const Shader * object)
{
    logObject(builder, object, "Shader");
    return builder;
}

LogMessageBuilder operator<<(LogMessageBuilder builder, const Texture * object)
{
    logObject(builder, object, "Texture");
    return builder;
}

LogMessageBuilder operator<<(LogMessageBuilder builder, const TransformFeedback * object)
{
    logObject(builder, object, "TransformFeedback");
    return builder;
}

LogMessageBuilder operator<<(LogMessageBuilder builder, const VertexArrayObject * object)
{
    logObject(builder, object, "VertexArrayObject");
    return builder;
}

LogMessageBuilder operator<<(LogMessageBuilder builder, const Sync * sync)
{
    builder << "Sync(" << sync->sync() << ")";

    return builder;
}

LogMessageBuilder operator<<(LogMessageBuilder builder, const AbstractUniform * uniform)
{
    builder << "AbstractUniform" << "(";
    if (uniform->identity().isName())
        builder << uniform->identity().name();
    else
        builder << uniform->identity().location();
    builder << ")";

    return builder;
}

} // namespace glow
