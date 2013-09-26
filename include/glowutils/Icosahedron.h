#pragma once

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

public:
    Icosahedron();

    void draw();

private:
    VertexArrayObject m_vao;

    Buffer m_vertices;
    Buffer m_indices;
};

} // namespace glow
