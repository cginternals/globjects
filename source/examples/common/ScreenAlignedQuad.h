#pragma once

#include <memory>

namespace globjects
{

class VertexArray;
class Program;
class Buffer;
class Texture;
class Shader;

template <typename T>
class Uniform;

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
    std::unique_ptr<globjects::VertexArray> m_vao;

    std::unique_ptr<globjects::Buffer> m_buffer;

    std::unique_ptr<globjects::Shader> m_vertexShader;
    std::unique_ptr<globjects::Shader> m_fragmentShader;

    std::unique_ptr<globjects::Program> m_program;
    std::unique_ptr<globjects::Texture> m_texture;

    std::unique_ptr<globjects::Uniform<unsigned int>> m_samplerUniform;

    int m_samplerIndex;

protected:
    static const char * s_defaultVertexShaderSource;
    static const char * s_defaultFagmentShaderSource;
};
