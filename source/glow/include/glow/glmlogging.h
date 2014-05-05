#pragma once

#include <glow/glow_api.h>

#include <glowbase/LogMessageBuilder.h>

#include <glm/glm.hpp>

namespace glow {

GLOW_API LogMessageBuilder operator<<(LogMessageBuilder builder, const glm::vec2 & vector);
GLOW_API LogMessageBuilder operator<<(LogMessageBuilder builder, const glm::vec3 & vector);
GLOW_API LogMessageBuilder operator<<(LogMessageBuilder builder, const glm::vec4 & vector);

GLOW_API LogMessageBuilder operator<<(LogMessageBuilder builder, const glm::ivec2 & vector);
GLOW_API LogMessageBuilder operator<<(LogMessageBuilder builder, const glm::ivec3 & vector);
GLOW_API LogMessageBuilder operator<<(LogMessageBuilder builder, const glm::ivec4 & vector);

GLOW_API LogMessageBuilder operator<<(LogMessageBuilder builder, const glm::uvec2 & vector);
GLOW_API LogMessageBuilder operator<<(LogMessageBuilder builder, const glm::uvec3 & vector);
GLOW_API LogMessageBuilder operator<<(LogMessageBuilder builder, const glm::uvec4 & vector);

GLOW_API LogMessageBuilder operator<<(LogMessageBuilder builder, const glm::mat2 & matrix);
GLOW_API LogMessageBuilder operator<<(LogMessageBuilder builder, const glm::mat3 & matrix);
GLOW_API LogMessageBuilder operator<<(LogMessageBuilder builder, const glm::mat4 & matrix);

} // namespace glow
