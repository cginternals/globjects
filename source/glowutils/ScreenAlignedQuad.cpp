
#include <glow/Program.h>
#include <glow/VertexArrayObject.h>
#include <glow/Texture.h>
#include <glow/Buffer.h>
#include <glow/Shader.h>

#include <glowutils/ScreenAlignedQuad.h>

namespace glow 
{

const char* ScreenAlignedQuad::s_defaultVertexShaderSource = R"(
#version 330

void main()
{
    gl_Position = vec4(0.0, 0.0, 0.0, 1.0);
}
)";

const char* ScreenAlignedQuad::s_defaultGeometryShaderSource = R"(
#version 330

layout (points) in;
layout (triangle_strip, max_vertices = 4) out;

out vec2 coordinates;

void main()
{
    gl_Position = gl_in[0].gl_Position + vec4(-1.0, -1.0, 0.0, 0.0);
    coordinates = vec2(0.0, 0.0);
    EmitVertex();

    gl_Position = gl_in[0].gl_Position + vec4(1.0, -1.0, 0.0, 0.0);
    coordinates = vec2(1.0, 0.0);
    EmitVertex();

    gl_Position = gl_in[0].gl_Position + vec4(-1.0, 1.0, 0.0, 0.0);
    coordinates = vec2(0.0, 1.0);
    EmitVertex();

    gl_Position = gl_in[0].gl_Position + vec4(1.0, 1.0, 0.0, 0.0);
    coordinates = vec2(1.0, 1.0);
    EmitVertex();
}

)";

const char* ScreenAlignedQuad::s_defaultFagmentShaderSource = R"(
#version 330

uniform sampler2D texture;

layout (location=0) out vec4 output;

in vec2 coordinates;

void main()
{
	output = texture2D(texture, coordinates);
}
)";

ScreenAlignedQuad::ScreenAlignedQuad(
    Shader * fragmentShader
,   Texture * texture)
:   m_vertexShader  (nullptr)
,   m_geometryShader(nullptr)
,   m_fragmentShader(fragmentShader)
,   m_program(new Program())
,   m_texture(texture)
{
    m_vertexShader   = Shader::fromString(GL_VERTEX_SHADER, s_defaultVertexShaderSource);
    m_geometryShader = Shader::fromString(GL_GEOMETRY_SHADER, s_defaultGeometryShaderSource);
    if (!m_fragmentShader)
        m_fragmentShader = Shader::fromString(GL_FRAGMENT_SHADER, s_defaultFagmentShaderSource);

    m_program->attach(m_vertexShader, m_geometryShader, m_fragmentShader);

    initialize();
}

ScreenAlignedQuad::ScreenAlignedQuad(Shader * fragmentShader)
:   ScreenAlignedQuad(fragmentShader, nullptr)
{
}

ScreenAlignedQuad::ScreenAlignedQuad(Texture * texture)
:   ScreenAlignedQuad(nullptr, texture)
{
}

ScreenAlignedQuad::ScreenAlignedQuad(Program * program)
:   m_vertexShader(nullptr)
,   m_geometryShader(nullptr)
,   m_fragmentShader(nullptr)
,   m_texture(nullptr)
,   m_program(program)
{
    initialize();
}

void ScreenAlignedQuad::initialize()
{
    m_vao = new VertexArrayObject;
    m_buffer = new Buffer(GL_ARRAY_BUFFER);
    
    const glm::vec4 point;
    m_buffer->setData(sizeof(glm::vec4), &point, GL_STATIC_DRAW); //needed for some drivers

    setSamplerUniform(0);
}

void ScreenAlignedQuad::draw()
{
    glClear(GL_COLOR_BUFFER_BIT);

	if (m_texture)
	{
        glActiveTexture(GL_TEXTURE0 + m_samplerIndex);
		m_texture->bind();
	}

    m_program->use();
    m_vao->drawArrays(GL_POINTS, 0, 1);
    m_program->release();

	if (m_texture)
	{
		m_texture->unbind();
    }
}

void ScreenAlignedQuad::setTexture(Texture* texture)
{
	m_texture = texture;
}

void ScreenAlignedQuad::setSamplerUniform(int index)
{
	m_samplerIndex = index;
	m_program->setUniform("texture", m_samplerIndex);
}

Program * ScreenAlignedQuad::program()
{
	return m_program;
}

Shader * ScreenAlignedQuad::vertexShader()
{
    return m_vertexShader;
}

Shader * ScreenAlignedQuad::geometryShader()
{
    return m_geometryShader;
}

Shader * ScreenAlignedQuad::fragmentShader()
{
    return m_fragmentShader;
}

} // namespace glow
