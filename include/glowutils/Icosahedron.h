#pragma once

#include <unordered_map>

#include <glm/glm.hpp>

#include <glow/Referenced.h>
#include <glow/ref_ptr.h>
#include <glow/Array.h>

#include <glowutils/glowutils.h>

namespace glow
{

class VertexArrayObject;
class Buffer;

}

namespace glowutils 
{

class GLOWUTILS_API Icosahedron : public glow::Referenced
{
public:
    static const glow::Array<glm::vec3> vertices();
    static const glow::Array<glm::lowp_uvec3> indices(); /// individual triangle indices (no strip, no fan)

   /**  Iterative triangle refinement: split each triangle into 4 new ones and 
        create points and indices appropriately.
    */
    static void refine(
        glow::Array<glm::vec3> & vertices
    ,   glow::Array<glm::lowp_uvec3> & indices
    ,   unsigned char levels);

public:
    Icosahedron(
        GLsizei iterations = 0
    ,   GLuint vertexAttribLocation = 0);

    virtual ~Icosahedron();


    /** draws the icosahedron as single triangles (TODO: generate set of triangle strips?)
    */
    void draw(GLenum mode = GL_TRIANGLES);

private:
    /** Splits a triangle edge by adding an appropriate new point (normalized
        on sphere) to the points (if not already cached) and returns the index
        to this point.
    */
    static glm::lowp_uint split(
        glm::lowp_uint a
    ,   glm::lowp_uint b
    ,   glow::Array<glm::vec3> & points
    ,   std::unordered_map<glm::uint, glm::lowp_uint> & cache);

private:
    glow::ref_ptr<glow::VertexArrayObject> m_vao;

    glow::ref_ptr<glow::Buffer> m_vertices;
    glow::ref_ptr<glow::Buffer> m_indices;

    GLsizei m_size;
};

} // namespace glowutils
