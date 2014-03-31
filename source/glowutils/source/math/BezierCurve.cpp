#include <glowutils/math/BezierCurve.h>

namespace glowutils {
namespace math {

const glm::mat4 BezierCurve::s_bersteinMatrix = glm::mat4(
     1,  0,  0,  0,
    -3,  3,  0,  0,
     3, -6,  3,  0,
    -1,  3, -3,  1
);

BezierCurve::BezierCurve()
{
}

BezierCurve::BezierCurve(const std::array<glm::vec3, 4>& controlPoints)
: m_controlPoints(controlPoints)
{
}

BezierCurve::BezierCurve(const glm::vec3& p0, const glm::vec3& p1, const glm::vec3& p2, const glm::vec3& p3)
: BezierCurve(std::array<glm::vec3, 4>{ { p0, p1, p2, p3 } })
{
}

glm::vec3 BezierCurve::operator()(float t) const
{
    return bezier(m_controlPoints[0], m_controlPoints[1], m_controlPoints[2], m_controlPoints[3], t);
}

glm::vec3 BezierCurve::bezier(const glm::vec3& p0, const glm::vec3& p1, const glm::vec3& p2, const glm::vec3& p3, float t)
{
    auto coefficients = s_bersteinMatrix * glm::vec4(1, t, t*t, t*t*t);

    return
        coefficients[0] * p0 +
        coefficients[1] * p1 +
        coefficients[2] * p2 +
        coefficients[3] * p3;
}

} // namespace math
} // namespace glowutils
