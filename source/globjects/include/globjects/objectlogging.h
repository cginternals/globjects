
#pragma once

#include <globjects/globjects_api.h>

#include <globjects/base/LogMessageBuilder.h>


namespace globjects
{


class Object;
class Buffer;
class Framebuffer;
class Program;
class Query;
class Renderbuffer;
class Sampler;
class Shader;
class Sync;
class Texture;
class TransformFeedback;
class VertexArray;
class AbstractUniform;
template <typename T>
class Uniform;

GLOBJECTS_API LogMessageBuilder operator<<(LogMessageBuilder builder, const Object * object);
GLOBJECTS_API LogMessageBuilder operator<<(LogMessageBuilder builder, const Buffer * object);
GLOBJECTS_API LogMessageBuilder operator<<(LogMessageBuilder builder, const Framebuffer * object);
GLOBJECTS_API LogMessageBuilder operator<<(LogMessageBuilder builder, const Program * object);
GLOBJECTS_API LogMessageBuilder operator<<(LogMessageBuilder builder, const Query * object);
GLOBJECTS_API LogMessageBuilder operator<<(LogMessageBuilder builder, const Renderbuffer * object);
GLOBJECTS_API LogMessageBuilder operator<<(LogMessageBuilder builder, const Sampler * object);
GLOBJECTS_API LogMessageBuilder operator<<(LogMessageBuilder builder, const Shader * object);
GLOBJECTS_API LogMessageBuilder operator<<(LogMessageBuilder builder, const Texture * object);
GLOBJECTS_API LogMessageBuilder operator<<(LogMessageBuilder builder, const TransformFeedback * object);
GLOBJECTS_API LogMessageBuilder operator<<(LogMessageBuilder builder, const VertexArray * object);
GLOBJECTS_API LogMessageBuilder operator<<(LogMessageBuilder builder, const Sync * sync);
GLOBJECTS_API LogMessageBuilder operator<<(LogMessageBuilder builder, const AbstractUniform * uniform);

template <typename T>
LogMessageBuilder operator<<(LogMessageBuilder builder, const Uniform<T> * uniform);
template <typename T>
LogMessageBuilder operator<<(LogMessageBuilder builder, Uniform<T> * uniform);


} // namespace globjects


#include <globjects/objectlogging.inl>
