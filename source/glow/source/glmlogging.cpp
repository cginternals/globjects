#include <glow/glmlogging.h>

#include <glm/vec2.hpp>
#include <glm/vec3.hpp>
#include <glm/vec4.hpp>
#include <glm/mat2x2.hpp>
#include <glm/mat3x3.hpp>
#include <glm/mat4x4.hpp>

namespace glow {

LogMessageBuilder operator<<(LogMessageBuilder builder, const glm::vec2 & vector)
{
    builder << "vec2(" << vector.x << ", " << vector.y << ")";

    return builder;
}

LogMessageBuilder operator<<(LogMessageBuilder builder, const glm::vec3 & vector)
{
    builder << "vec3(" << vector.x << ", " << vector.y << ", " << vector.z << ")";

    return builder;
}

LogMessageBuilder operator<<(LogMessageBuilder builder, const glm::vec4 & vector)
{
    builder << "vec4(" << vector.x << ", " << vector.y << ", " << vector.z << ", " << vector.w  << ")";

    return builder;
}

LogMessageBuilder operator<<(LogMessageBuilder builder, const glm::ivec2 & vector)
{
    builder << "ivec2(" << vector.x << ", " << vector.y << ")";

    return builder;
}

LogMessageBuilder operator<<(LogMessageBuilder builder, const glm::ivec3 & vector)
{
    builder << "ivec3(" << vector.x << ", " << vector.y << ", " << vector.z << ")";

    return builder;
}

LogMessageBuilder operator<<(LogMessageBuilder builder, const glm::ivec4 & vector)
{
    builder << "ivec4(" << vector.x << ", " << vector.y << ", " << vector.z << ", " << vector.w  << ")";

    return builder;
}

LogMessageBuilder operator<<(LogMessageBuilder builder, const glm::uvec2 & vector)
{
    builder << "uvec2(" << vector.x << ", " << vector.y << ")";

    return builder;
}

LogMessageBuilder operator<<(LogMessageBuilder builder, const glm::uvec3 & vector)
{
    builder << "uvec3(" << vector.x << ", " << vector.y << ", " << vector.z << ")";

    return builder;
}

LogMessageBuilder operator<<(LogMessageBuilder builder, const glm::uvec4 & vector)
{
    builder << "uvec4(" << vector.x << ", " << vector.y << ", " << vector.z << ", " << vector.w  << ")";

    return builder;
}

LogMessageBuilder operator<<(LogMessageBuilder builder, const glm::mat2 & matrix)
{
    builder
        << "mat2("
        << "(" << matrix[0][0] << ", " << matrix[0][1] << "), "
        << "(" << matrix[1][0] << ", " << matrix[1][1] << ")"
        << ")";

    return builder;
}

LogMessageBuilder operator<<(LogMessageBuilder builder, const glm::mat3 & matrix)
{
    builder
        << "mat3("
        << "(" << matrix[0][0] << ", " << matrix[0][1] << ", " << matrix[0][2] << "), "
        << "(" << matrix[1][0] << ", " << matrix[1][1] << ", " << matrix[1][2] <<"), "
        << "(" << matrix[2][0] << ", " << matrix[2][1] << ", " << matrix[2][2] <<")"
        << ")";

    return builder;
}

LogMessageBuilder operator<<(LogMessageBuilder builder, const glm::mat4 & matrix)
{
    builder
        << "mat4("
        << "(" << matrix[0][0] << ", " << matrix[0][1] << ", " << matrix[0][2] << ", " << matrix[0][3] << "), "
        << "(" << matrix[1][0] << ", " << matrix[1][1] << ", " << matrix[1][2] << ", " << matrix[1][3] <<"), "
        << "(" << matrix[2][0] << ", " << matrix[2][1] << ", " << matrix[2][2] << ", " << matrix[2][3] <<"), "
        << "(" << matrix[3][0] << ", " << matrix[3][1] << ", " << matrix[3][2] << ", " << matrix[3][3] <<")"
        << ")";

    return builder;
}

} // namespace glow
