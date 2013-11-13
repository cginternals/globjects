
#include <glow/Shader.h>
#include <cassert>

#include <glowutils/File.h>

#include <glowutils/FileRegistry.h>
#include <glowutils/RawFile.h>

namespace glow
{

Shader* createShaderFromFile(const GLenum type, const std::string& filename)
{
    return new Shader(type, new File(filename));
}

File::File(const std::string & filePath)
:   m_filePath(filePath)
{
    RawFile<char> raw(m_filePath);
    if (raw.valid())
        m_source = std::string(raw.data(), raw.size());

    FileRegistry::instance().registerFile(this);
}

File::~File()
{
    FileRegistry::instance().deregisterFile(this);
}

const std::string & File::string() const
{
	return m_source;
}

std::string File::shortInfo() const
{
    return filePath();
}

const std::string& File::filePath() const
{
	return m_filePath;
}

void File::reload()
{
    RawFile<char> raw(m_filePath);
    if (raw.valid())
    {
        m_source = std::string(raw.data(), raw.size());
	    changed();
    }
}

} // namespace glow
