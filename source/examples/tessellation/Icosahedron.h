#pragma once

#include <unordered_map>
#include <array>
#include <vector>

#include <glm/glm.hpp>

#include <glbinding/gl/gl.h>

#include <globjects/base/Referenced.h>
#include <globjects/base/ref_ptr.h>


namespace globjects
{

class VertexArray;
class Buffer;

}


class Icosahedron : public globjects::Referenced
{
public:
    using Face = std::array<gl::GLushort, 3>;

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
        gl::GLsizei iterations = 0
    ,   const gl::GLint positionLocation = 0
    ,   const gl::GLint normalLocation = 1);

    /** draws the icosahedron as single triangles (TODO: generate set of triangle strips?)
    */
    void draw();
    void draw(gl::GLenum mode);

protected:
    /** Splits a triangle edge by adding an appropriate new point (normalized
        on sphere) to the points (if not already cached) and returns the index
        to this point.
    */
    static gl::GLushort split(
        gl::GLushort a
    ,   gl::GLushort b
    ,   std::vector<glm::vec3> & points
    ,   std::unordered_map<glm::uint, gl::GLushort> & cache);

protected:
    virtual ~Icosahedron();

protected:
    globjects::ref_ptr<globjects::VertexArray> m_vao;

    globjects::ref_ptr<globjects::Buffer> m_vertices;
    globjects::ref_ptr<globjects::Buffer> m_indices;

    gl::GLsizei m_size;
};
