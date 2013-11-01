#pragma once

#include <glm/glm.hpp>

namespace glow
{

/** \brief Provides a mapping from screen coordinates to world coordinates.

    This interface is inteded to be used by, e.g., navigation classes that require
    basic scene information based on the pointing devices (mouse, touch, ...) position.

    \see WorldInHandNavigation
*/
class AbstractCoordinateProvider
{
public:
    virtual const float depthAt(const glm::ivec2 & windowCoordinates) = 0;

    virtual const glm::vec3 objAt(
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
