#include <glowutils/UnitCube.h>

#include <cmath>

#include <glm/glm.hpp>

#include <glow/VertexArrayObject.h>
#include <glow/VertexAttributeBinding.h>
#include <glow/Buffer.h>
#include <glow/Error.h>

using namespace glm;
using namespace glow;

namespace glowutils 
{

const std::array<glm::vec3, 28> UnitCube::strip()
{
    std::array<glm::vec3, 8> vertices{{
        vec3(-.5f,-.5f,-.5f)
    ,   vec3(-.5f,-.5f, .5f)
    ,   vec3(-.5f, .5f,-.5f)
    ,   vec3(-.5f, .5f, .5f)
    ,   vec3( .5f,-.5f,-.5f)
    ,   vec3( .5f,-.5f, .5f)
    ,   vec3( .5f, .5f,-.5f)
    ,   vec3( .5f, .5f, .5f)
    }};

    std::array<glm::vec3, 7> normals{{
        vec3(-1, 0, 0)  
    ,   vec3( 1, 0, 0)  
    ,   vec3( 0,-1, 0)  
    ,   vec3( 0, 1, 0)  
    ,   vec3( 0, 0,-1)  
    ,   vec3( 0, 0, 1)  
    ,   vec3( 0, 0, 0)  // dummy
    }};

    // use an interleaved array
    return std::array<glm::vec3, 28>{{
        vertices[7], normals[6]
    ,   vertices[3], normals[6]
    ,   vertices[5], normals[5]
    ,   vertices[1], normals[5]
    ,   vertices[0], normals[2]
    ,   vertices[3], normals[0]
    ,   vertices[2], normals[0]
    ,   vertices[7], normals[3]
    ,   vertices[6], normals[3]
    ,   vertices[5], normals[1]
    ,   vertices[4], normals[1]
    ,   vertices[0], normals[2]
    ,   vertices[6], normals[4] 
    ,   vertices[2], normals[4]
    }};
}

UnitCube::UnitCube(const GLuint vertexAttribLocation, const GLuint normalAttribLocation)
: m_strip(new Buffer(GL_ARRAY_BUFFER))
, m_vao(new VertexArrayObject)
{
    m_strip->setData(strip(), GL_STATIC_DRAW);

    m_vao->bind();

    auto vertexBinding = m_vao->binding(0);
    vertexBinding->setAttribute(vertexAttribLocation);
    vertexBinding->setBuffer(m_strip.get(), 0, static_cast<GLint>(sizeof(vec3) * 2));
    vertexBinding->setFormat(3, GL_FLOAT, GL_FALSE, 0);
    m_vao->enable(0);

    auto normalBinding = m_vao->binding(1);
    normalBinding->setAttribute(normalAttribLocation);
    normalBinding->setBuffer(m_strip.get(), 0, static_cast<GLint>(sizeof(vec3) * 2));
    normalBinding->setFormat(3, GL_FLOAT, GL_TRUE, sizeof(vec3));
    m_vao->enable(1);

    m_vao->unbind();
}

void UnitCube::draw()
{
    glEnable(GL_DEPTH_TEST);
    CheckGLError();

    m_vao->bind();
    m_vao->drawArrays(GL_TRIANGLE_STRIP, 0, 14);
    m_vao->unbind();
}

} // namespace glowutils
