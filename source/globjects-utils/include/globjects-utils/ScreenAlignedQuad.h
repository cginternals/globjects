#pragma once

#include <globjects-base/Referenced.h>
#include <globjects-base/ref_ptr.h>

#include <globjects/VertexArrayObject.h>
#include <globjects/Program.h>
#include <globjects/Buffer.h>
#include <globjects/Texture.h>
#include <globjects/Shader.h>

#include <globjects-utils/globjects-utils_api.h>

namespace gloutils
{

class GLOBJECTS_UTILS_API ScreenAlignedQuad : public glo::Referenced
{
public:
    ScreenAlignedQuad(glo::Shader * fragmentShader, glo::Texture * texture);

    ScreenAlignedQuad(glo::Texture * texture);
    ScreenAlignedQuad(glo::Shader  * fragmentShader);
    ScreenAlignedQuad(glo::Program * program);

	void draw();

    glo::Program * program();

    glo::Shader * vertexShader();
    glo::Shader * fragmentShader();

    void setTexture(glo::Texture * texture);

	void setSamplerUniform(int index);

protected:
    void initialize();

protected:
    glo::ref_ptr<glo::VertexArrayObject> m_vao;

    glo::ref_ptr<glo::Buffer> m_buffer;

    glo::ref_ptr<glo::Shader> m_vertexShader;
    glo::ref_ptr<glo::Shader> m_fragmentShader;

    glo::ref_ptr<glo::Program> m_program;
    glo::ref_ptr<glo::Texture> m_texture;

    int m_samplerIndex;

protected:
	static const char * s_defaultVertexShaderSource;
	static const char * s_defaultFagmentShaderSource;
};

} // namespace gloutils
