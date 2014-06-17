#pragma once

#include <array>

#include <glbinding/types.h>

#include <glm/glm.hpp>

#include <glowbase/ref_ptr.h>
#include <glowbase/Referenced.h>

#include <glow/VertexArrayObject.h>
#include <glow/Buffer.h>

#include <glowutils/glowutils_api.h>

namespace glowutils 
{

class GLOWUTILS_API UnitCube : public glow::Referenced
{
    static const std::array<glm::vec3, 28> strip();

public:
    UnitCube(gl::GLuint vertexAttribLocation = 0, gl::GLuint normalAttribLocation = 1);

    /** draws the unit cube as single triangle strip. Note: interleaved array 
        is used, so the normal comes from the provoking vertex (expected to be
        the default gl::glLAST_VERTEX_CONVENTION). use flat interpolation to get 
        the face normal within the fragment shader.
    */
    void draw();

private:
    glow::ref_ptr<glow::Buffer> m_strip;
    glow::ref_ptr<glow::VertexArrayObject> m_vao;
};

} // namespace glowutils
