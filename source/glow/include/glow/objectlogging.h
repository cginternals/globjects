#pragma once

#include <glow/glow_api.h>

#include <glowbase/LogMessageBuilder.h>

namespace glow
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

GLOW_API LogMessageBuilder operator<<(LogMessageBuilder builder, const Object * object);
GLOW_API LogMessageBuilder operator<<(LogMessageBuilder builder, const Buffer * object);
GLOW_API LogMessageBuilder operator<<(LogMessageBuilder builder, const FrameBufferObject * object);
GLOW_API LogMessageBuilder operator<<(LogMessageBuilder builder, const Program * object);
GLOW_API LogMessageBuilder operator<<(LogMessageBuilder builder, const Query * object);
GLOW_API LogMessageBuilder operator<<(LogMessageBuilder builder, const RenderBufferObject * object);
GLOW_API LogMessageBuilder operator<<(LogMessageBuilder builder, const Sampler * object);
GLOW_API LogMessageBuilder operator<<(LogMessageBuilder builder, const Shader * object);
GLOW_API LogMessageBuilder operator<<(LogMessageBuilder builder, const Texture * object);
GLOW_API LogMessageBuilder operator<<(LogMessageBuilder builder, const TransformFeedback * object);
GLOW_API LogMessageBuilder operator<<(LogMessageBuilder builder, const VertexArrayObject * object);

GLOW_API LogMessageBuilder operator<<(LogMessageBuilder builder, const Sync * sync);
GLOW_API LogMessageBuilder operator<<(LogMessageBuilder builder, const AbstractUniform * uniform);

template <typename T>
LogMessageBuilder operator<<(LogMessageBuilder builder, const Uniform<T> * uniform);

} // namespace glow

#include <glow/objectlogging.hpp>
