#pragma once

#include <unordered_map>
#include <array>
#include <vector>

#include <GL/glew.h>

#include <glm/glm.hpp>

#include <glow/Referenced.h>
#include <glow/ref_ptr.h>

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
    using Face = std::array<u_int16_t, 3>;

    static const std::array<glm::vec3, 12> vertices();
    static const std::array<Face, 20> indices(); /// individual triangle indices (no strip, no fan)

   /**  Iterative triangle refinement: split each triangle into 4 new ones and 
        create points and indices appropriately.
    */
    static void refine(
        std::vector<glm::vec3> & vertices
    ,   std::vector<Face> & indices
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
    static u_int16_t split(
        u_int16_t a
    ,   u_int16_t b
    ,   std::vector<glm::vec3> & points
    ,   std::unordered_map<glm::uint, u_int16_t> & cache);

private:
    glow::ref_ptr<glow::VertexArrayObject> m_vao;

    glow::ref_ptr<glow::Buffer> m_vertices;
    glow::ref_ptr<glow::Buffer> m_indices;

    GLsizei m_size;
};

} // namespace glowutils
