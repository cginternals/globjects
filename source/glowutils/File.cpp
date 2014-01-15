
#include <glow/Shader.h>
#include <cassert>

#include <glowutils/File.h>

#include <glowutils/FileRegistry.h>
#include <glowutils/RawFile.h>

using namespace glow;

namespace glowutils
{

File::File(const std::string & filePath)
: m_filePath(filePath)
, m_valid(false)
{
    FileRegistry::instance().registerFile(this);
}

File::~File()
{
    FileRegistry::instance().deregisterFile(this);
}

std::string File::string() const
{
    if (!m_valid)
        loadFileContent();

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
    m_valid = false;
    changed();
}

void File::loadFileContent() const
{
    RawFile<char> raw(m_filePath);
    if (raw.valid())
    {
        m_source = std::string(raw.data(), raw.size());
    }
    else
    {
        m_source = "";
    }

    m_valid = true;
}

} // namespace glowutils
