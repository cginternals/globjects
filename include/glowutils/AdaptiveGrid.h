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


/** \brief A plane aligned regular raster with camera position based refinement.

    This grid alignes itself along the given input plane. It tries to stay visible 
    for as long as possible, by moving the fixed number of segments with the camera's
    position and adjusting the sub grid based on the camera's distance to the given 
    plane. Additionally, the grid is rendered with a smooth circular mask centered 
    at the perpendicular to the plane. Also near and far planes are considered, 
    causing a smooth alpha transition near the far plane. 

    Note: Any camera change causing view frustum changes, needs to be passed to 
    the grid inn order to work correctly. The grid is drawn using transparency 
    with OpenGL blending and depth buffer enabled.

    Typeical usage of the AdaptiveGrid:
    \code{.cpp}
    
        AdaptiveGrid * agrid = new AdaptiveGrid(16U);
        ...
        agrid->setNearFar(0.1f, 1024.f);
        agrid->update(eye, projection * view);
        agrid->draw();

    \endcode
*/
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
