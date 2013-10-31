#pragma once

#include <glm/glm.hpp>

#include <glow/Array.h>
#include <glow/Referenced.h>

#include <glowutils/glowutils.h>

namespace glow 
{

class VertexArrayObject;
class Buffer;

class GLOWUTILS_API UnitCube : public Referenced
{
    static const Array<glm::vec3> strip();

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
    ref_ptr<VertexArrayObject> m_vao;
    ref_ptr<Buffer> m_strip;
};

} // namespace glow
