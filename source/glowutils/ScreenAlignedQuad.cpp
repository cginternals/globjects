#include <glowutils/ScreenAlignedQuad.h>

#include <cassert>
#include <array>

#include <glow/Program.h>
#include <glow/VertexArrayObject.h>
#include <glow/Texture.h>
#include <glow/VertexAttributeBinding.h>
#include <glow/Buffer.h>
#include <glow/Shader.h>
#include <glow/StaticStringSource.h>

#include <glowutils/StringTemplate.h>

using namespace glow;

namespace glowutils 
{

const char * ScreenAlignedQuad::s_defaultVertexShaderSource = R"(
#version 140
#extension GL_ARB_explicit_attrib_location : require

layout (location = 0) in vec2 a_vertex;
out vec2 v_uv;

void main()
{
	v_uv = a_vertex * 0.5 + 0.5;
	gl_Position = vec4(a_vertex, 0.0, 1.0);
}
)";

const char* ScreenAlignedQuad::s_defaultFagmentShaderSource = R"(
#version 140
#extension GL_ARB_explicit_attrib_location : require

uniform sampler2D source;

layout (location = 0) out vec4 fragColor;

in vec2 v_uv;

void main()
{
    fragColor = texture(source, v_uv);
}
)";

ScreenAlignedQuad::ScreenAlignedQuad(
    Shader * fragmentShader
,   Texture * texture)
:   m_vertexShader  (nullptr)
,   m_fragmentShader(fragmentShader)
,   m_program(new Program())
,   m_texture(texture)
,   m_samplerIndex(0)
{
    
    glowutils::StringTemplate* vertexShaderSource = new glowutils::StringTemplate(new StaticStringSource(s_defaultVertexShaderSource));
    glowutils::StringTemplate* fragmentShaderSource = new glowutils::StringTemplate(new StaticStringSource(s_defaultFagmentShaderSource));
    
    
#ifdef MAC_OS
    vertexShaderSource->replace("#version 140", "#version 150");
    fragmentShaderSource->replace("#version 140", "#version 150");
#endif
    
    m_vertexShader   = new Shader(GL_VERTEX_SHADER, vertexShaderSource);
    
    if (!m_fragmentShader)
        m_fragmentShader = new Shader(GL_FRAGMENT_SHADER, fragmentShaderSource);

    m_program->attach(m_vertexShader, m_fragmentShader);

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
,   m_fragmentShader(nullptr)
,   m_program(program)
,   m_texture(nullptr)
,   m_samplerIndex(0)
{
    assert(program != nullptr);

    initialize();
}

void ScreenAlignedQuad::initialize()
{
	// By default, counterclockwise polygons are taken to be front-facing.
	// http://www.opengl.org/sdk/docs/man/xhtml/glFrontFace.xml

    static const std::array<glm::vec2, 4> raw{{
		glm::vec2( +1.f, -1.f )
	,	glm::vec2( +1.f, +1.f )
	,	glm::vec2( -1.f, -1.f )
	,	glm::vec2( -1.f, +1.f )
    }};

    m_vao = new VertexArrayObject;
    m_buffer = new Buffer(GL_ARRAY_BUFFER);
    m_buffer->setData(raw, GL_STATIC_DRAW); //needed for some drivers

	auto binding = m_vao->binding(0);
	binding->setAttribute(0);
	binding->setBuffer(m_buffer, 0, sizeof(glm::vec2));
	binding->setFormat(2, GL_FLOAT, GL_FALSE, 0);
	m_vao->enable(0);

    setSamplerUniform(0);
}

void ScreenAlignedQuad::draw()
{
    if (m_texture)
	{
        glActiveTexture(GL_TEXTURE0 + m_samplerIndex);
        CheckGLError();

        m_texture->bind();
	}

    m_program->use();
    m_vao->drawArrays(GL_TRIANGLE_STRIP, 0, 4);
    m_program->release();

	if (m_texture)
		m_texture->unbind();
}

void ScreenAlignedQuad::setTexture(Texture* texture)
{
	m_texture = texture;
}

void ScreenAlignedQuad::setSamplerUniform(int index)
{
	m_samplerIndex = index;
	m_program->setUniform("source", m_samplerIndex);
}

Program * ScreenAlignedQuad::program()
{
	return m_program;
}

Shader * ScreenAlignedQuad::vertexShader()
{
    return m_vertexShader;
}

Shader * ScreenAlignedQuad::fragmentShader()
{
    return m_fragmentShader;
}

} // namespace glowutils
