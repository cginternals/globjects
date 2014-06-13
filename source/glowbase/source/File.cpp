#include <glowbase/File.h>

#include <fstream>

#include <glowbase/baselogging.h>

#include "FileRegistry.h"

namespace glow
{

File::File(const std::string & filePath)
: m_filePath(filePath)
, m_valid(false)
{
    FileRegistry::registerFile(this);
}

File::~File()
{
    FileRegistry::deregisterFile(this);
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

void File::reloadAll()
{
    FileRegistry::reloadAll();
}

void File::loadFileContent() const
{
    std::ifstream ifs(m_filePath, std::ios::in | std::ios::binary | std::ios::ate);

    if (ifs)
    {
        const std::streamsize size = static_cast<const std::streamsize>(ifs.tellg());

        ifs.seekg(0, std::ios::beg);

        m_source.resize(size);

        ifs.read(const_cast<char*>(m_source.data()), size);
        ifs.close();

        m_valid = true;
    }
    else
    {
        glow::warning() << "Reading from file \"" << m_filePath << "\" failed.";

        m_source = "";

        m_valid = false;
    }
}

} // namespace glowutils
