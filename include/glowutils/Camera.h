#pragma once

#include <glm/glm.hpp>

#include <glowutils/glowutils.h>
#include <glowutils/CachedValue.h>


namespace glow
{

class GLOWUTILS_API Camera
{
public:
    Camera(
        const glm::vec3 & eye    = glm::vec3(0.0, 0.0, 1.0)
    ,   const glm::vec3 & center = glm::vec3(0.0, 0.0, 0.0) 
    ,   const glm::vec3 & up     = glm::vec3(0.0, 1.0, 0.0));

    virtual ~Camera();

    const glm::vec3 & eye() const;
    void setEye(const glm::vec3 & eye);
    const glm::vec3 & center() const;
    void setCenter(const glm::vec3 & center);
    const glm::vec3 & up() const;
    void setUp(const glm::vec3 & up);

    float zNear() const;
    void setZNear(float zNear);
    float zFar() const;
    void setZFar(float zFar);

    float fovy() const;
    void setFovy(float fovy);

    const glm::ivec2 & viewport() const;
    void setViewport(const glm::ivec2 & viewport);

    float aspectRatio() const;

    // lazy matrices getters

    const glm::mat4 & view();
    const glm::mat4 & projection();
    const glm::mat4 & viewProjection();
    const glm::mat4 & viewInverted();
    const glm::mat4 & projectionInverted();
    const glm::mat4 & viewProjectionInverted();

    void update();

    virtual void changed();

protected:
    void dirty(bool update = true);
    void invalidateMatrices();

protected:
    bool m_dirty;

    glm::vec3 m_eye;
    glm::vec3 m_center;
    glm::vec3 m_up;

    float m_fovy;
    float m_aspect;
    float m_zNear;
    float m_zFar;

    glm::ivec2 m_viewport;

    CachedValue<glm::mat4> m_view;
    CachedValue<glm::mat4> m_viewInverted;
    CachedValue<glm::mat4> m_projection;
    CachedValue<glm::mat4> m_projectionInverted;
    CachedValue<glm::mat4> m_viewProjection;
    CachedValue<glm::mat4> m_viewProjectionInverted;
};

} // namespace glow
