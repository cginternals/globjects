#include <globjects/objectlogging.h>

#include <globjects/Object.h>
#include <globjects/Buffer.h>
#include <globjects/Framebuffer.h>
#include <globjects/Program.h>
#include <globjects/Query.h>
#include <globjects/Renderbuffer.h>
#include <globjects/Sampler.h>
#include <globjects/Shader.h>
#include <globjects/Sync.h>
#include <globjects/Texture.h>
#include <globjects/TransformFeedback.h>
#include <globjects/VertexArray.h>
#include <globjects/AbstractUniform.h>

namespace globjects {

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

LogMessageBuilder operator<<(LogMessageBuilder builder, const Framebuffer * object)
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

LogMessageBuilder operator<<(LogMessageBuilder builder, const Renderbuffer * object)
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

LogMessageBuilder operator<<(LogMessageBuilder builder, const VertexArray * object)
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

} // namespace globjects
