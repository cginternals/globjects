
#include <glow/Shader.h>
#include <cassert>

#include <glowutils/ShaderFile.h>

#include <glowutils/ShaderFileRegistry.h>
#include <glowutils/RawFile.h>

namespace glow
{

Shader* createShaderFromFile(const GLenum type, const std::string& filename)
{
    return new Shader(type, new ShaderFile(filename));
}

ShaderFile::ShaderFile(const std::string & filePath)
:   m_filePath(filePath)
{
    RawFile<char> raw(m_filePath);
    if (raw.valid())
        m_source = std::string(raw.data(), raw.size());

    ShaderFileRegistry::instance().registerFile(this);
}

ShaderFile::~ShaderFile()
{
    ShaderFileRegistry::instance().deregisterFile(this);
}

const std::string & ShaderFile::source() const
{
	return m_source;
}

std::string ShaderFile::shortInfo() const
{
    return filePath();
}

const std::string& ShaderFile::filePath() const
{
	return m_filePath;
}

void ShaderFile::reload()
{
    RawFile<char> raw(m_filePath);
    if (raw.valid())
    {
        m_source = std::string(raw.data(), raw.size());
	    changed();
    }
}

} // namespace glow
