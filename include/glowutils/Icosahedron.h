#pragma once

#include <hash_map>

#include <glm/glm.hpp>

#include <glow/Array.h>
#include <glow/VertexArrayObject.h>
#include <glow/Buffer.h>
#include <glow/Referenced.h>

#include <glowutils/glowutils.h>

namespace glow 
{

class GLOWUTILS_API Icosahedron : public Referenced
{
public:
    static const Array<glm::vec3> vertices();
    static const Array<glm::lowp_uvec3> indices(); /// individual triangle indices (no strip, no fan)

   /**  Iterative triangle refinement: split each triangle into 4 new ones and 
        create points and indices appropriately.
    */
    static void refine(
        Array<glm::vec3> & vertices
    ,   Array<glm::lowp_uvec3> & indices
    ,   const unsigned char levels);

public:
    Icosahedron(const GLsizei iterations = 0);

    /** draws the icosahedron as single triangles (TODO: generate set of triangle strips?)
    */
    void draw();

private:
    /** Splits a triangle edge by adding an appropriate new point (normalized
        on sphere) to the points (if not already cached) and returns the index
        to this point.
    */
    static const glm::lowp_uint split(
        const glm::lowp_uint a
    ,   const glm::lowp_uint b
    ,   Array<glm::vec3> & points
    ,   std::hash_map<glm::uint, glm::lowp_uint> & cache);

private:
    VertexArrayObject m_vao;

    Buffer m_vertices;
    Buffer m_indices;

    GLsizei m_size;
};

} // namespace glow
