#pragma once

#include <array>

#include <glm/glm.hpp>


namespace glowutils {
namespace math {

class BezierCurve
{
public:
    BezierCurve();
    BezierCurve(const std::array<glm::vec3, 4>& controlPoints);
    BezierCurve(const glm::vec3& p0, const glm::vec3& p1, const glm::vec3& p2, const glm::vec3& p3);

    glm::vec3 operator()(float t) const;

    static glm::vec3 bezier(const glm::vec3& p0, const glm::vec3& p1, const glm::vec3& p2, const glm::vec3& p3, float t);
protected:
    std::array<glm::vec3, 4> m_controlPoints;

    static const glm::mat4 s_bersteinMatrix;
};

} // namespace math
} // namespace glowutils
