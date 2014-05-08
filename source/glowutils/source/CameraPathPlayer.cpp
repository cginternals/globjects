#include <glowutils/CameraPathPlayer.h>

#include <cassert>
#include <string>
#include <algorithm>
#include <functional>
#include <limits>
#include <vector>

#include <glm/gtc/type_ptr.hpp>

#include <glow/Buffer.h>
#include <glow/Program.h>
#include <glow/VertexArrayObject.h>
#include <glow/VertexAttributeBinding.h>
#include <glow/logging.h>
#include <glow/StaticStringSource.h>

#include <glowutils/Camera.h>
#include <glowutils/StringTemplate.h>
#include <glowutils/math/BezierCurve.h>

using namespace glm;

namespace glowutils
{

CameraPathPlayer::CameraPathPlayer(Camera & camera)
: m_camera(camera)
, m_bufferSize(0)
{
}

void CameraPathPlayer::setPath(const CameraPath & path)
{
   m_path = path;
   prepare();
}

void CameraPathPlayer::prepare()
{
    const std::vector<CameraPathPoint>& points = m_path.points();

    if (points.size()<=1)
        return;

    float totalLength = 0.0;
    const CameraPathPoint* previous = &points[0];

    for (unsigned int i = 1; i<points.size(); ++i)
    {
        const CameraPathPoint* current = &points[i];
        float distance = glm::length(current->eye - previous->eye);

        float startT = totalLength;
        float endT = startT + distance;

        PathSection section{ previous, current, startT, endT, glm::vec3(), glm::vec3() };
        m_sections.push_back(section);

        totalLength = endT;
        previous = current;
    }

    /*{
        const CameraPathPoint* current = &points[0];
        float distance = glm::length(current->eye - previous->eye);

        float startT = totalLength;
        float endT = startT + distance;

        PathSection section{ previous, current, startT, endT };
        m_sections.push_back(section);

        totalLength = endT;
    }*/

    // normalize
    for (PathSection& section : m_sections)
    {
        section.startT /= totalLength;
        section.endT /= totalLength;
    }

    prepareControlPoints();
}

namespace {
vec3 intersection(const vec3& a, const vec3& r, const vec3& p, const vec3& n)
{
    float rDotN = dot(r, n);

    assert(std::abs(rDotN) < std::numeric_limits<float>::epsilon());

    float t = dot(p - a, n) / rDotN;
    return a + r * t;
}

const char* vertexSource = R"(
#version 140
#extension GL_ARB_explicit_attrib_location : require

uniform mat4 transform;

layout(location = 0) in vec4 vertex;

out float t;

void main()
{
    gl_Position = transform * vec4(vertex.xyz, 1.0);
    t = vertex.w;

}
)";

const char* fragmentSource = R"(
#version 140
#extension GL_ARB_explicit_attrib_location : require

layout(location=0) out vec4 fragColor;

in float t;

void main()
{
    fragColor = vec4(vec3(1.0, 0.2+t*0.8, t/2.0), 1.0);
}
)";

}

void CameraPathPlayer::prepareControlPoints()
{
    const std::vector<CameraPathPoint>& points = m_path.points();

    const CameraPathPoint* previous = &points[0];
    const CameraPathPoint* current = &points[1];

    //const CameraPathPoint* first = &points[0];
    const CameraPathPoint* last = &points[points.size()-1];

    std::vector<glm::vec3> dirs;
    vec3 dir0 = current->eye - previous->eye;
    dir0 = (dir0 + (previous->eye - (&points[points.size()-2])->eye))/2.f;
    dirs.push_back(dir0);

    for (unsigned int i = 1; i<points.size()-1; ++i)
    {
        const CameraPathPoint* current = &points[i];
        const CameraPathPoint* next = &points[i+1];

        vec3 t1 = current->eye - previous->eye;
        vec3 t2 = next->eye - current->eye;

        vec3 dirI = (t1+t2)/2.f;
        dirs.push_back(dirI);

        previous = current;
    }

    vec3 dirLast = last->eye - previous->eye;
    dirLast = (dirLast + ((&points[1])->eye - last->eye))/2.f;
    dirs.push_back(dirLast);

    int i = 0;
    for (PathSection& section : m_sections)
    {
        vec3 d1 = glm::normalize(dirs[i]);
        vec3 d2 = glm::normalize(dirs[i+1]);

        vec3 p1 = section.start->eye;
        vec3 p2 = section.end->eye;

        float f = glm::length(p2-p1)/2.7f;

        section.c1 = p1 + d1 * f;
        section.c2 = p2 - d2 * f;

        i++;
    }
}

CameraPathPlayer::PathSection& CameraPathPlayer::find(const float t)
{
    return *std::lower_bound(m_sections.begin(), m_sections.end(), t, [](const PathSection& section, float value) {
        return section.endT < value;
    });
}

CameraPathPoint CameraPathPlayer::interpolate(const PathSection& section, const float t)
{
    const CameraPathPoint& p1 = *section.start;
    const CameraPathPoint& p2 = *section.end;

    vec3 eye = math::BezierCurve(p1.eye, section.c1, section.c2, p2.eye)(t);

    //glowbase::debug() << eye << " (" << (p1.eye*(1.0f-t) + p2.eye*t) << ")";

    //eye = p1.eye*(1.0f-t) + p2.eye*t;

    return CameraPathPoint(
                eye,
                p1.center*(1.0f-t) + p2.center*t,
                p1.up*(1.0f-t) + p2.up*t,
                p1.fov*(1.0f-t) + p2.fov*t);
}

void CameraPathPlayer::moveCamera(const CameraPathPoint& point)
{
    m_camera.setCenter(point.center);
    m_camera.setEye(point.eye);
    m_camera.setUp(point.up);
    m_camera.setFovy(point.fov);
}

void CameraPathPlayer::play(const float t)
{
    const float clampedT = glm::clamp(t, 0.f, 1.f);

    PathSection& section = find(clampedT);
    const float sectionT = (clampedT - section.startT) / (section.endT - section.startT);

    moveCamera(interpolate(section, sectionT));
}

void CameraPathPlayer::createVao()
{
    m_bufferSize = 100;

    m_buffer = new glow::Buffer();

    std::vector<vec4> array;

    for (int i=0; i<m_bufferSize; ++i)
    {
        float t = float(i)/float(m_bufferSize-1);
        PathSection& section = find(t);
        const float sectionT = (t - section.startT) / (section.endT - section.startT);
        const CameraPathPoint& p1 = *section.start;
        const CameraPathPoint& p2 = *section.end;
        vec3 pos = math::BezierCurve(p1.eye, section.c1, section.c2, p2.eye)(sectionT);
        array.emplace_back(pos, t);
    }

    m_buffer->setData(array);

    m_vao = new glow::VertexArrayObject();

    m_vao->binding(0)->setBuffer(m_buffer, 0, sizeof(vec4));
    m_vao->binding(0)->setFormat(4, gl::FLOAT);
    m_vao->binding(0)->setAttribute(0);

    m_vao->enable(0);

    m_program = new glow::Program();

    StringTemplate* vertexShaderSource = new glowutils::StringTemplate(new glow::StaticStringSource(vertexSource));
    StringTemplate* fragmentShaderSource = new glowutils::StringTemplate(new glow::StaticStringSource(fragmentSource));

#ifdef MAC_OS
    vertexShaderSource->replace("#version 140", "#version 150");
    fragmentShaderSource->replace("#version 140", "#version 150");
#endif

    m_program->attach(
        new glow::Shader(gl::VERTEX_SHADER, vertexShaderSource),
        new glow::Shader(gl::FRAGMENT_SHADER, fragmentShaderSource)
    );

    m_program->addUniform(new glow::Uniform<mat4>("transform"));
}

void CameraPathPlayer::draw(const mat4& viewProjection)
{
    m_program->setUniform("transform", viewProjection);

    m_program->use();
    m_vao->bind();
    m_vao->drawArrays(gl::LINE_STRIP, 0, m_bufferSize);
    m_vao->unbind();
    m_program->release();
}

void CameraPathPlayer::freeVao()
{
    m_vao = nullptr;
    m_program = nullptr;
    m_buffer = nullptr;
}

} // namespace glowutils
