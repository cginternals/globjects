#pragma once

#include <unordered_map>
#include <array>
#include <vector>

#include <glbinding/gl/enum.h>

#include <glm/glm.hpp>

#include <globjects-base/Referenced.h>
#include <globjects-base/ref_ptr.h>

#include <globjects/VertexArray.h>
#include <globjects/Buffer.h>

#include <globjects-utils/globjects-utils_api.h>

namespace gloutils 
{

class GLOBJECTS_UTILS_API Icosahedron : public glo::Referenced
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
    void draw(gl::GLenum mode = gl::GL_TRIANGLES);

private:
    /** Splits a triangle edge by adding an appropriate new point (normalized
        on sphere) to the points (if not already cached) and returns the index
        to this point.
    */
    static gl::GLushort split(
        gl::GLushort a
    ,   gl::GLushort b
    ,   std::vector<glm::vec3> & points
    ,   std::unordered_map<glm::uint, gl::GLushort> & cache);

private:
    glo::ref_ptr<glo::VertexArray> m_vao;

    glo::ref_ptr<glo::Buffer> m_vertices;
    glo::ref_ptr<glo::Buffer> m_indices;

    gl::GLsizei m_size;
protected:
    virtual ~Icosahedron();
};

} // namespace gloutils
