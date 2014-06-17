#include <glowutils/AdaptiveGrid.h>

#include <vector>

#include <glbinding/functions.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/transform.hpp>

#include <glowbase/StaticStringSource.h>

#include <glow/Program.h>
#include <glow/Shader.h>
#include <glow/Buffer.h>
#include <glow/VertexArrayObject.h>
#include <glow/VertexAttributeBinding.h>

#include <glowutils/Plane3.h>
#include <glowutils/Camera.h>
#include <glowutils/StringTemplate.h>

using namespace glm;
using namespace glow;

namespace glowutils 
{

const char * AdaptiveGrid::s_vsSource = R"(

#version 140
#extension GL_ARB_explicit_attrib_location : require

uniform mat4 transform;
uniform vec2 viewPlaneDistance;

layout (location = 0) in vec4 a_vertex;

flat out float v_type;
out vec3 v_vertex;

void main()
{
    float m = 1.0 - viewPlaneDistance[1];
    float t = a_vertex.w;

    vec4 vertex = transform * vec4(a_vertex.xyz, 1.0);
    v_vertex = vertex.xyz;

    // interpolate minor grid lines alpha based on viewPlaneDistance
    v_type =  mix(1.0 - t, 1.0 - 2.0 * m * t, step(a_vertex.w, 0.7998));

    gl_Position = vertex;
}

)";

const char * AdaptiveGrid::s_fsSource = R"(

#version 140
#extension GL_ARB_explicit_attrib_location : require

uniform vec2 viewPlaneDistance;

uniform float znear;
uniform float zfar;
uniform vec3 color;

flat in float v_type;
in vec3 v_vertex;

layout (location = 0) out vec4 fragColor;

void main()
{
    float t = v_type;

    float z = gl_FragCoord.z;

    // complete function
    // z = (2.0 * zfar * znear / (zfar + znear - (zfar - znear) * (2.0 * z - 1.0)));
    // normalized to [0,1]
    // z = (z - znear) / (zfar - znear);

    // simplyfied with wolfram alpha
    z = - znear * z / (zfar * z - zfar - znear * z);

    float g = mix(t, 1.0, z * z);

    float l = clamp(8.0 - length(v_vertex) / viewPlaneDistance[0], 0.0, 1.0);

    fragColor = vec4(color, l * (1.0 - g * g));
}

)";


AdaptiveGrid::AdaptiveGrid(
    unsigned short segments
,   const vec3 & location
,   const vec3 & normal)
:   m_program(new Program())
,   m_vao(nullptr)
,   m_buffer(nullptr)
,   m_camera(nullptr)
,   m_location(location)
,   m_normal(normal)
,   m_size(0)
{
    m_transform = transform(m_location, m_normal);
  
    StringTemplate* vertexShaderString = new StringTemplate(new glow::StaticStringSource(s_vsSource));
    StringTemplate* fragmentShaderString = new StringTemplate(new glow::StaticStringSource(s_fsSource));
  
  
#ifdef MAC_OS
  vertexShaderString->replace("#version 140", "#version 150");
  fragmentShaderString->replace("#version 140", "#version 150");
#endif
  
  
    m_program->attach(new Shader(gl::GL_VERTEX_SHADER, vertexShaderString),
                      new Shader(gl::GL_FRAGMENT_SHADER, fragmentShaderString));

    setColor(vec3(.8f));

    setupGridLineBuffer(segments);

    m_vao = new VertexArrayObject;
    auto binding = m_vao->binding(0);

    binding->setAttribute(0);
    binding->setBuffer(m_buffer, 0, sizeof(vec4));
    binding->setFormat(4, gl::GL_FLOAT, gl::GL_FALSE, 0);

    m_vao->enable(0);
}

AdaptiveGrid::~AdaptiveGrid()
{
}

void AdaptiveGrid::setupGridLineBuffer(unsigned short segments)
{
    std::vector<vec4> points;

    float type;
    int  n = 1;

    const float g(static_cast<float>(segments));

    type = .2f; // sub gridlines, every 0.125, except every 0.5
    for (float f = -g + .125f; f < g; f += .125f)
    {
        if (n++ % 4)
        {
            points.emplace_back(g, 0.f, f, type);
            points.emplace_back(-g, 0.f, f, type);
            points.emplace_back(f, 0.f, g, type);
            points.emplace_back(f, 0.f,-g, type);
        }
    }
    type = .4f; // grid lines every 1.0 units, offseted by 0.5
    for (float f = -g + .5f; f < g; f += 1.f)
    {
        points.emplace_back(g, 0.f, f, type);
        points.emplace_back(-g, 0.f, f, type);
        points.emplace_back(f, 0.f, g, type);
        points.emplace_back(f, 0.f,-g, type);
    }
    type = .8f; // grid lines every 1.0 units
    for (float f = -g + 1.f; f < g; f += 1.f) 
    {
        points.emplace_back(g, 0.f, f, type);
        points.emplace_back(-g, 0.f, f, type);
        points.emplace_back(f, 0.f, g, type);
        points.emplace_back(f, 0.f,-g, type);
    }

    // use hesse normal form and transform each grid line onto the specified plane.
    mat4 T; // ToDo;
    for (vec4 & point : points)
        point = vec4(vec3(T * vec4(point.x, point.y, point.z, 1.f)), point.w);

    m_buffer = new Buffer();
    m_buffer->setData(points, gl::GL_STATIC_DRAW);

    m_size = static_cast<unsigned short>(segments * 64 - 4);
}

void AdaptiveGrid::setCamera(const Camera * camera)
{
    m_camera = camera;
    update();
}

void AdaptiveGrid::setNearFar(
    float zNear
,   float zFar)
{
    m_program->setUniform("znear", zNear);
    m_program->setUniform("zfar", zFar);
}

void AdaptiveGrid::setColor(const vec3 & color)
{
    m_program->setUniform("color", color);
}

void AdaptiveGrid::update()
{
    if (!m_camera)
        return;

    setNearFar(m_camera->zNear(), m_camera->zFar());
    update(m_camera->eye(), m_camera->viewProjection());
}

void AdaptiveGrid::update(
    const vec3 & viewpoint
,   const mat4 & modelViewProjection)
{
    // Project the camera's eye position onto the grid plane.
    bool intersects; //  should always intersect.
    const vec3 i = intersection(intersects, m_location, m_normal
        , viewpoint, viewpoint - m_normal);

    // This transforms the horizontal plane vectors u and v  onto the actual 
    // grid plane. Than express the intersection i as a linear combination of 
    // u and v by solving the linear equation system.
    // Finally round the s and t values to get the nearest major grid point.

    const float l = length(viewpoint - i);

    const float distancelog = log(l * .5f) / log(8.f);
    const float viewPlaneDistance = pow(8.f, ceil(distancelog));

    mat4 T; // ToDo;

    const vec3 u(vec3(T * vec4(viewPlaneDistance, 0.f, 0.f, 1.f)) - m_location);
    const vec3 v(vec3(T * vec4(0.f, 0.f, viewPlaneDistance, 1.f)) - m_location);

    const int j = (u[0] < 0.0f || u[0] > 0.0f) ? 0 : (u[1] < 0.0 || u[1] > 0.0) ? 1 : 2;
    const int k = (v[0] < 0.0 || v[0] > 0.0) && j != 0 ? 0 : (v[1] < 0.0 || v[1] > 0.0) && j != 1 ? 1 : 2;

    const vec3 a(i - m_location);

    const float t = v[k] > 0.0 || v[k] < 0.0 ? a[k] / v[k] : 0.f;
    const float s = u[j] > 0.0 || u[j] < 0.0 ? (a[j] - t * v[j]) / u[j] : 0.f;

    const vec3 irounded = round(s) * u + round(t) * v;

    const mat4 offset = translate(irounded) * scale(vec3(viewPlaneDistance));

    m_program->setUniform("viewPlaneDistance",  vec2(l, mod(distancelog, 1.f)));
    m_program->setUniform("transform", modelViewProjection * offset);
} 

void AdaptiveGrid::draw()
{
    gl::glBlendFunc(gl::GL_SRC_ALPHA, gl::GL_ONE_MINUS_SRC_ALPHA);

    gl::glEnable(gl::GL_BLEND);

    gl::glEnable(gl::GL_DEPTH_TEST);


    m_program->use();

    m_vao->bind();
    m_vao->drawArrays(gl::GL_LINES, 0, m_size);
    m_vao->unbind();

    m_program->release();

    gl::glDisable(gl::GL_BLEND);

}

} // namespace glowutils
