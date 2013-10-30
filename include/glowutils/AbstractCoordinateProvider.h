#pragma once

#include <glm/glm.hpp>

namespace glow
{

class AbstractCoordinateProvider
{
public:
    virtual const float depthAt(const glm::ivec2 & windowCoordinates) = 0;

    virtual const glm::vec3 objat(
        const glm::ivec2 & windowCoordinates) = 0;

    virtual const glm::vec3 objAt(
        const glm::ivec2 & windowCoordinates
        , const float depth) = 0;

    virtual const glm::vec3 objAt(
        const glm::ivec2 & windowCoordinates
        , const float depth
        , const glm::mat4 & viewProjectionInverted) = 0;
};

} // namespace glow
