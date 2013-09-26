
#include <cmath>

#include <glm/glm.hpp>

#include <glowutils/Icosahedron.h>


using namespace glm;

namespace glow 
{

const Array<glm::vec3> Icosahedron::vertices()
{
    static const float t = (1.f + sqrtf(5.f)) * 0.5f; // 2.118
    static const float i = inversesqrt(t * t + 1.f);  // 0.427
    static const float a = t * i;                     // 0.904

    // icosahedron vertices (normalized) form three orthogonal golden rectangles
    // http://en.wikipedia.org/wiki/Icosahedron#Cartesian_coordinates

    return Array<vec3>
    {
        vec3(-i, a, 0) 
    ,   vec3( i, a, 0)
    ,   vec3(-i,-a, 0)
    ,   vec3( i,-a, 0)
    ,   vec3( 0,-i, a)
    ,   vec3( 0, i, a)
    ,   vec3( 0,-i,-a)
    ,   vec3( 0, i,-a)
    ,   vec3( a, 0,-i)
    ,   vec3( a, 0, i)
    ,   vec3(-a, 0,-i)
    ,   vec3(-a, 0, i)
    };
}

const Array<glm::lowp_uvec3> Icosahedron::indices()
{
    return Array<lowp_uvec3> 
    {
        lowp_uvec3(  0, 11,  5)
    ,   lowp_uvec3(  0,  5,  1)
    ,   lowp_uvec3(  0,  1,  7)
    ,   lowp_uvec3(  0,  7, 10)
    ,   lowp_uvec3(  0, 10, 11)

    ,   lowp_uvec3(  1,  5,  9)
    ,   lowp_uvec3(  5, 11,  4)
    ,   lowp_uvec3( 11, 10,  2)
    ,   lowp_uvec3( 10,  7,  6)
    ,   lowp_uvec3(  7,  1,  8)

    ,   lowp_uvec3(  3,  9,  4)
    ,   lowp_uvec3(  3,  4,  2)
    ,   lowp_uvec3(  3,  2,  6)
    ,   lowp_uvec3(  3,  6,  8)
    ,   lowp_uvec3(  3,  8,  9)

    ,   lowp_uvec3(  4,  9,  5)
    ,   lowp_uvec3(  2,  4, 11)
    ,   lowp_uvec3(  6,  2, 10)
    ,   lowp_uvec3(  8,  6,  7)
    ,   lowp_uvec3(  9,  8,  1)
    };
}

Icosahedron::Icosahedron()
:   m_indices(GL_ELEMENT_ARRAY_BUFFER)
,   m_vertices(GL_ARRAY_BUFFER)
{
    m_indices.setData(indices(), GL_STATIC_DRAW);
    m_vertices.setData(vertices(), GL_STATIC_DRAW);

    m_vao.bind();

    auto vertexBinding = m_vao.binding(0);
    vertexBinding->setAttribute(0);
    vertexBinding->setBuffer(&m_vertices, 0, sizeof(vec3));
    vertexBinding->setFormat(3, GL_FLOAT);
    m_vao.enable(0);

    m_indices.bind();

    m_vao.unbind();
}

void Icosahedron::draw()
{
    glEnable(GL_DEPTH_TEST);

    m_vao.bind();
    m_vao.drawElements(GL_TRIANGLES, 60, GL_UNSIGNED_SHORT, nullptr);
    m_vao.unbind();
}

} // namespace glow
