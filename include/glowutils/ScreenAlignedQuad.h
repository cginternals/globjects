#pragma once

#include <glow/Program.h>
#include <glow/VertexArrayObject.h>
#include <glow/Texture.h>
#include <glow/Buffer.h>
#include <glow/Array.h>

namespace glow
{

class ScreenAlignedQuad : public Referenced
{
public:
    ScreenAlignedQuad(
        Shader * fragmentShader = nullptr
    ,   Texture * texture = nullptr);

	ScreenAlignedQuad(Texture * texture);
	ScreenAlignedQuad(Shader  * fragmentShader);
    ScreenAlignedQuad(Program * program);

	void draw();

	Program* program();
	void setProgram(Program* program);

    Shader* vertexShader();
    Shader* geometryShader();
    Shader* fragmentShader();

	void setTexture(Texture* texture);

	void setSamplerUniform(int index);

protected:
    void initialize();

protected:
	ref_ptr<Program> m_program;
	ref_ptr<Texture> m_texture;

    ref_ptr<Shader> m_vertexShader;
    ref_ptr<Shader> m_geometryShader;
    ref_ptr<Shader> m_fragmentShader;

    ref_ptr<VertexArrayObject> m_vao;

    ref_ptr<Buffer> m_buffer;

    int m_samplerIndex;

protected:
	static const char * s_defaultVertexShaderSource;
    static const char * s_defaultGeometryShaderSource;
	static const char * s_defaultFagmentShaderSource;
};

} // namespace glow
