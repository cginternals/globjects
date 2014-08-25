#pragma once

#include <globjects/globjects_api.h>

#include <globjects-base/LogMessageBuilder.h>

namespace glo
{

class Object;
class Buffer;
class FrameBufferObject;
class Program;
class Query;
class RenderBufferObject;
class Sampler;
class Shader;
class Sync;
class Texture;
class TransformFeedback;
class VertexArrayObject;
class AbstractUniform;
template <typename T>
class Uniform;
class Version;

GLOBJECTS_API LogMessageBuilder operator<<(LogMessageBuilder builder, const Object * object);
GLOBJECTS_API LogMessageBuilder operator<<(LogMessageBuilder builder, const Buffer * object);
GLOBJECTS_API LogMessageBuilder operator<<(LogMessageBuilder builder, const FrameBufferObject * object);
GLOBJECTS_API LogMessageBuilder operator<<(LogMessageBuilder builder, const Program * object);
GLOBJECTS_API LogMessageBuilder operator<<(LogMessageBuilder builder, const Query * object);
GLOBJECTS_API LogMessageBuilder operator<<(LogMessageBuilder builder, const RenderBufferObject * object);
GLOBJECTS_API LogMessageBuilder operator<<(LogMessageBuilder builder, const Sampler * object);
GLOBJECTS_API LogMessageBuilder operator<<(LogMessageBuilder builder, const Shader * object);
GLOBJECTS_API LogMessageBuilder operator<<(LogMessageBuilder builder, const Texture * object);
GLOBJECTS_API LogMessageBuilder operator<<(LogMessageBuilder builder, const TransformFeedback * object);
GLOBJECTS_API LogMessageBuilder operator<<(LogMessageBuilder builder, const VertexArrayObject * object);

GLOBJECTS_API LogMessageBuilder operator<<(LogMessageBuilder builder, const Sync * sync);
GLOBJECTS_API LogMessageBuilder operator<<(LogMessageBuilder builder, const AbstractUniform * uniform);

template <typename T>
LogMessageBuilder operator<<(LogMessageBuilder builder, const Uniform<T> * uniform);
template <typename T>
LogMessageBuilder operator<<(LogMessageBuilder builder, Uniform<T> * uniform);

} // namespace glo

#include <globjects/objectlogging.hpp>
