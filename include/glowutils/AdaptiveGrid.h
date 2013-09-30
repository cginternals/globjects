#pragma once

#include <glm/glm.hpp>

#include <glow/ref_ptr.hpp>
#include <glow/Referenced.h>

#include <glowutils/glowutils.h>


namespace glow 
{

class Program;
class VertexArrayObject;
class Buffer;


class GLOWUTILS_API AdaptiveGrid : public Referenced 
{
public:
    AdaptiveGrid(
        unsigned short segments = 16
    ,   const glm::vec3 & location = glm::vec3(0.f, 0.f, 0.f)
    ,   const glm::vec3 & normal = glm::vec3(0.f, 1.f, 0.f));

    virtual ~AdaptiveGrid();

    void setNearFar(
        float zNear
    ,   float zFar);

    void setColor(const glm::vec3 & color);

    void update(
        const glm::vec3 & viewpoint
    ,   const glm::mat4 & modelViewProjection);

    void draw();

private:
    void setupGridLineBuffer(unsigned short segments);

private:
    static const char * s_vsSource;
    static const char * s_fsSource;

private:
    Program * m_program;

    VertexArrayObject * m_vao;
    Buffer * m_buffer;

    glm::vec3 m_location;
    glm::vec3 m_normal;

    glm::mat4 m_transform;

    unsigned short m_size;
};

} // namespace glow
