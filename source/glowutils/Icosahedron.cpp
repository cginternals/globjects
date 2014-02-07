#include <glowutils/Icosahedron.h>

#include <cmath>
#include <iterator>
#include <algorithm>

#include <glm/glm.hpp>

#include <glow/VertexArrayObject.h>
#include <glow/VertexAttributeBinding.h>
#include <glow/Buffer.h>
#include <glow/Error.h>

using namespace glm;
using namespace glow;

namespace glowutils 
{

const std::array<vec3, 12> Icosahedron::vertices()
{
    static const float t = (1.f + sqrtf(5.f)) * 0.5f; // 2.118
    static const float i = inversesqrt(t * t + 1.f);  // 0.427
    static const float a = t * i;                     // 0.904

    // icosahedron vertices (normalized) form three orthogonal golden rectangles
    // http://en.wikipedia.org/wiki/Icosahedron#Cartesian_coordinates

    return std::array<vec3, 12>{{
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
    }};
}

const std::array<lowp_uvec3, 20> Icosahedron::indices()
{
    return std::array<lowp_uvec3, 20>{{
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
    }};
}

Icosahedron::Icosahedron(
    const GLsizei iterations
,   const GLuint vertexAttribLocation)
:   m_vao(new VertexArrayObject)
,   m_vertices(new Buffer(GL_ARRAY_BUFFER))
,   m_indices(new Buffer(GL_ELEMENT_ARRAY_BUFFER))
{
    auto v(vertices());
    auto i(indices());

    std::vector<vec3> vertices(v.begin(), v.end());
    std::vector<lowp_uvec3> indices(i.begin(), i.end());

    refine(vertices, indices, static_cast<char>(clamp(iterations, 0, 8)));

    m_indices->setData(indices, GL_STATIC_DRAW);
    m_vertices->setData(vertices, GL_STATIC_DRAW);

    m_size = static_cast<GLsizei>(indices.size() * 3);

    m_vao->bind();

    auto vertexBinding = m_vao->binding(0);
    vertexBinding->setAttribute(vertexAttribLocation);
    vertexBinding->setBuffer(m_vertices, 0, sizeof(vec3));
    vertexBinding->setFormat(3, GL_FLOAT, GL_TRUE);
    m_vao->enable(0);

    m_indices->bind();

    m_vao->unbind();
}

Icosahedron::~Icosahedron()
{
}

void Icosahedron::draw(const GLenum mode)
{
    glEnable(GL_DEPTH_TEST);
    CheckGLError();

    m_vao->bind();
    m_vao->drawElements(mode, m_size, GL_UNSIGNED_SHORT, nullptr);
    m_vao->unbind();

    // glDisable(GL_DEPTH_TEST); // TODO: Use stackable states
}

void Icosahedron::refine(
    std::vector<vec3> & vertices
,   std::vector<lowp_uvec3> & indices
,   const unsigned char levels)
{
    std::unordered_map<uint, lowp_uint> cache;

    for(int i = 0; i < levels; ++i)
    {
        const int size(static_cast<int>(indices.size()));

        for(int f = 0; f < size; ++f)
        {
            glm::lowp_uvec3 & face(indices[f]);

            const glm::lowp_uint a(face.x);
            const glm::lowp_uint b(face.y);
            const glm::lowp_uint c(face.z);

            const glm::lowp_uint ab(split(a, b, vertices, cache));
            const glm::lowp_uint bc(split(b, c, vertices, cache));
            const glm::lowp_uint ca(split(c, a, vertices, cache));

            face = glm::lowp_uvec3(ab, bc, ca);

            indices.emplace_back(a, ab, ca);
            indices.emplace_back(b, bc, ab);
            indices.emplace_back(c, ca, bc);
        }
    }
}

lowp_uint Icosahedron::split(
    const lowp_uint a
,   const lowp_uint b
,   std::vector<vec3> & points
,   std::unordered_map<uint, lowp_uint> & cache)
{
    const bool aSmaller(a < b);

    const uint smaller(aSmaller ? a : b);
    const uint greater(aSmaller ? b : a);
    const uint hash((smaller << 16) + greater);

    auto h(cache.find(hash));
    if(cache.end() != h)
        return h->second;

    points.push_back(normalize((points[a] + points[b]) * .5f));

    const lowp_uint i = static_cast<lowp_uint>(points.size() - 1);

    cache[hash] = i;

    return i;
}

} // namespace glowutils
