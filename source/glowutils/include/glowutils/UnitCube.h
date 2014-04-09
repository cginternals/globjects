#pragma once

#include <array>

#include <GL/glew.h>

#include <glm/glm.hpp>

#include <glow/ref_ptr.h>
#include <glow/Referenced.h>

#include <glowutils/glowutils_api.h>

namespace glow
{
    class VertexArrayObject;
    class Buffer;
}

namespace glowutils 
{

class GLOWUTILS_API UnitCube : public glow::Referenced
{
    static const std::array<glm::vec3, 28> strip();

public:
    UnitCube(GLuint vertexAttribLocation = 0, GLuint normalAttribLocation = 1);

    /** draws the unit cube as single triangle strip. Note: interleaved array 
        is used, so the normal comes from the provoking vertex (expected to be
        the default GL_LAST_VERTEX_CONVENTION). use flat interpolation to get 
        the face normal within the fragment shader.
    */
    void draw();

private:
    glow::ref_ptr<glow::Buffer> m_strip;
    glow::ref_ptr<glow::VertexArrayObject> m_vao;
};

} // namespace glowutils
