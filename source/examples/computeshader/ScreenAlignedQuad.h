#pragma once


namespace globjects
{

class VertexArray;
class Program;
class Buffer;
class Texture;
class Shader;

}


class ScreenAlignedQuad
{
public:
    ScreenAlignedQuad(globjects::Shader * fragmentShader, globjects::Texture * texture);

    ScreenAlignedQuad(globjects::Texture * texture);
    ScreenAlignedQuad(globjects::Shader  * fragmentShader);
    ScreenAlignedQuad(globjects::Program * program);

    void draw();

    globjects::Program * program();

    globjects::Shader * vertexShader();
    globjects::Shader * fragmentShader();

    void setTexture(globjects::Texture * texture);

    void setSamplerUniform(int index);

protected:
    void initialize();

protected:
    globjects::VertexArray * m_vao;

    globjects::Buffer * m_buffer;

    globjects::Shader * m_vertexShader;
    globjects::Shader * m_fragmentShader;

    globjects::Program * m_program;
    globjects::Texture * m_texture;

    int m_samplerIndex;

protected:
    static const char * s_defaultVertexShaderSource;
    static const char * s_defaultFagmentShaderSource;
};
