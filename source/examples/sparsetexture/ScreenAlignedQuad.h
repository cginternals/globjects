#pragma once


#include <memory>

#include <globjects/base/Instantiator.h>


namespace globjects
{

class VertexArray;
class Program;
class Buffer;
class Texture;
class Shader;
class AbstractStringSource;

}


class ScreenAlignedQuad : public globjects::Instantiator<ScreenAlignedQuad>
{
public:
    ScreenAlignedQuad(globjects::Program * program, globjects::Texture * texture = nullptr);

    static std::unique_ptr<globjects::AbstractStringSource> vertexShaderSource();
    static std::unique_ptr<globjects::AbstractStringSource> fragmentShaderSource();

    void draw();

    globjects::Program * program();

    void setTexture(globjects::Texture * texture);

    void setSamplerUniform(int index);

protected:
    void initialize();

protected:
    std::unique_ptr<globjects::VertexArray> m_vao;
    std::unique_ptr<globjects::Buffer> m_buffer;

    globjects::Program * m_program;
    globjects::Texture * m_texture;

    int m_samplerIndex;

protected:
    static const char * s_defaultVertexShaderSource;
    static const char * s_defaultFagmentShaderSource;
};
