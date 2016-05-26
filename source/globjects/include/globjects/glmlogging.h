
#pragma once

#include <globjects/globjects_api.h>

#include <globjects/base/LogMessageBuilder.h>

#include <glm/fwd.hpp>


namespace globjects 
{


GLOBJECTS_API LogMessageBuilder operator<<(LogMessageBuilder builder, const glm::vec2 & vector);
GLOBJECTS_API LogMessageBuilder operator<<(LogMessageBuilder builder, const glm::vec3 & vector);
GLOBJECTS_API LogMessageBuilder operator<<(LogMessageBuilder builder, const glm::vec4 & vector);

GLOBJECTS_API LogMessageBuilder operator<<(LogMessageBuilder builder, const glm::ivec2 & vector);
GLOBJECTS_API LogMessageBuilder operator<<(LogMessageBuilder builder, const glm::ivec3 & vector);
GLOBJECTS_API LogMessageBuilder operator<<(LogMessageBuilder builder, const glm::ivec4 & vector);

GLOBJECTS_API LogMessageBuilder operator<<(LogMessageBuilder builder, const glm::uvec2 & vector);
GLOBJECTS_API LogMessageBuilder operator<<(LogMessageBuilder builder, const glm::uvec3 & vector);
GLOBJECTS_API LogMessageBuilder operator<<(LogMessageBuilder builder, const glm::uvec4 & vector);

GLOBJECTS_API LogMessageBuilder operator<<(LogMessageBuilder builder, const glm::mat2 & matrix);
GLOBJECTS_API LogMessageBuilder operator<<(LogMessageBuilder builder, const glm::mat3 & matrix);
GLOBJECTS_API LogMessageBuilder operator<<(LogMessageBuilder builder, const glm::mat4 & matrix);


} // namespace globjects
