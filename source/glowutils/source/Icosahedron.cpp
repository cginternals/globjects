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

const std::array<Icosahedron::Face, 20> Icosahedron::indices()
{
    return std::array<Face, 20>{{
        Face{{  0, 11,  5}}
    ,   Face{{  0,  5,  1}}
    ,   Face{{  0,  1,  7}}
    ,   Face{{  0,  7, 10}}
    ,   Face{{  0, 10, 11}}

    ,   Face{{  1,  5,  9}}
    ,   Face{{  5, 11,  4}}
    ,   Face{{ 11, 10,  2}}
    ,   Face{{ 10,  7,  6}}
    ,   Face{{  7,  1,  8}}

    ,   Face{{  3,  9,  4}}
    ,   Face{{  3,  4,  2}}
    ,   Face{{  3,  2,  6}}
    ,   Face{{  3,  6,  8}}
    ,   Face{{  3,  8,  9}}

    ,   Face{{  4,  9,  5}}
    ,   Face{{  2,  4, 11}}
    ,   Face{{  6,  2, 10}}
    ,   Face{{  8,  6,  7}}
    ,   Face{{  9,  8,  1}}
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
    std::vector<Face> indices(i.begin(), i.end());

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
,   std::vector<Face> & indices
,   const unsigned char levels)
{
    std::unordered_map<uint, GLushort> cache;

    for(int i = 0; i < levels; ++i)
    {
        const int size(static_cast<int>(indices.size()));

        for(int f = 0; f < size; ++f)
        {
            Face & face = indices[f];

            const GLushort a(face[0]);
            const GLushort b(face[1]);
            const GLushort c(face[2]);

            const GLushort ab(split(a, b, vertices, cache));
            const GLushort bc(split(b, c, vertices, cache));
            const GLushort ca(split(c, a, vertices, cache));

            face = { ab, bc, ca };

            indices.emplace_back(Face{ a, ab, ca });
            indices.emplace_back(Face{ b, bc, ab });
            indices.emplace_back(Face{ c, ca, bc });
        }
    }
}

GLushort Icosahedron::split(
    const GLushort a
,   const GLushort b
,   std::vector<vec3> & points
,   std::unordered_map<uint, GLushort> & cache)
{
    const bool aSmaller(a < b);

    const uint smaller(aSmaller ? a : b);
    const uint greater(aSmaller ? b : a);
    const uint hash((smaller << 16) + greater);

    auto h(cache.find(hash));
    if(cache.end() != h)
        return h->second;

    points.push_back(normalize((points[a] + points[b]) * .5f));

    const GLushort i = static_cast<GLushort>(points.size() - 1);

    cache[hash] = i;

    return i;
}

} // namespace glowutils
