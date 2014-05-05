#pragma once

#include <glowbase/Referenced.h>
#include <glowbase/ref_ptr.h>

#include <glowutils/glowutils_api.h>

namespace glow {

class Buffer;
class Program;
class Shader;
class Texture;
class VertexArrayObject;

}

namespace glowutils
{

class GLOWUTILS_API ScreenAlignedQuad : public glow::Referenced
{
public:
    ScreenAlignedQuad(glow::Shader * fragmentShader, glow::Texture * texture);

    ScreenAlignedQuad(glow::Texture * texture);
    ScreenAlignedQuad(glow::Shader  * fragmentShader);
    ScreenAlignedQuad(glow::Program * program);

	void draw();

    glow::Program * program();

    glow::Shader * vertexShader();
    glow::Shader * fragmentShader();

    void setTexture(glow::Texture * texture);

	void setSamplerUniform(int index);

protected:
    void initialize();

protected:
    glow::ref_ptr<glow::VertexArrayObject> m_vao;

    glow::ref_ptr<glow::Buffer> m_buffer;

    glow::ref_ptr<glow::Shader> m_vertexShader;
    glow::ref_ptr<glow::Shader> m_fragmentShader;

    glow::ref_ptr<glow::Program> m_program;
    glow::ref_ptr<glow::Texture> m_texture;

    int m_samplerIndex;

protected:
	static const char * s_defaultVertexShaderSource;
	static const char * s_defaultFagmentShaderSource;
};

} // namespace glowutils
