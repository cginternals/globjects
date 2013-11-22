#pragma once

#include <glm/glm.hpp>

#include <glow/Array.h>
#include <glow/ref_ptr.h>
#include <glow/Referenced.h>

#include <glowutils/glowutils.h>

namespace glow
{
    class VertexArrayObject;
    class Buffer;
}

namespace glowutils 
{

class GLOWUTILS_API UnitCube : public glow::Referenced
{
    static const glow::Array<glm::vec3> strip();

public:
    UnitCube(
        GLuint vertexAttribLocation = 0
    ,   GLuint normalAttribLocation = 1);

    /** draws the unit cube as single triangle strip. Note: interleaved array 
        is used, so the normal comes from the provoking vertex (expected to be
        the default GL_LAST_VERTEX_CONVENTION). use flat interpolation to get 
        the face normal within the fragment shader.
    */
    void draw();

private:
    glow::ref_ptr<glow::VertexArrayObject> m_vao;
    glow::ref_ptr<glow::Buffer> m_strip;
};

} // namespace glowutils
