#pragma once

#include <glm/glm.hpp>

#include <glowutils/glowutils_api.h>

namespace glowutils
{

class GLOWUTILS_API AxonometricLookAt
{
public:
    enum ProjectionType
    {
        Custom      // set horizontal/vertical angles as you like
    ,   Isometric30 // 30 degree Isometric projection (NEN/ISO)
    ,   Isometric12 // 1:2 Isometric projection (NEN/ISO)
    ,   Military    // isometric projection (no skewed z-plane)
    ,   Dimetric427 // dimetric projection
    ,   Chinese     // dimetric projection ("chinese")
    };

public:
    AxonometricLookAt(
        ProjectionType type = Isometric30
    ,   const glm::vec3 & position = glm::vec3()
    ,   float zoom = 1.f
    ,   const glm::mat4 & rotation = glm::mat4());

    AxonometricLookAt(
        float verticalAngle
    ,   float horizontalAngle
    ,   const glm::vec3 & position = glm::vec3()
    ,   float zoom = 1.f
    ,   const glm::mat4 & rotation = glm::mat4());

    const glm::mat4 & matrix() const;

    void setType(ProjectionType type);
    ProjectionType type() const;

    void setVerticalAngle(float angle);
    float verticalAngle() const;

    void setHorizontalAngle(float angle);
    float horizontalAngle() const;

    void setPosition(const glm::vec3 & position);
    const glm::vec3 & position() const;

    void setZoom(float zoom);
    float zoom() const;

    void setRotation(const glm::mat4 & rotation);
    const glm::mat4 & rotation() const;

private:
    void rebuild() const;

private:
    float   m_verticalAngle;
    float   m_horizontalAngle;

    glm::mat4 m_rotation;
    glm::vec3 m_position;

    float m_zoom;

    ProjectionType m_type;

    mutable bool m_rebuild;
    mutable glm::mat4 m_axonometric;
};

} // namespace glowutils
