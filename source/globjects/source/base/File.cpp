#include <globjects/base/File.h>

#include <fstream>

#include <globjects/base/baselogging.h>

#include "FileRegistry.h"

namespace globjects
{

File::File(const std::string & filePath, bool binary)
: m_filePath(filePath)
, m_binary(binary)
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
    std::ios::openmode mode = std::ios::in | std::ios::ate;
    if (m_binary) {
        mode |= std::ios::binary;
    }

    std::ifstream ifs(m_filePath, mode);

    if (ifs)
    {
        const std::streamsize size = static_cast<const std::streamsize>(ifs.tellg());

        ifs.seekg(0, std::ios::beg);

        m_source.resize(size);

        ifs.read(const_cast<char*>(m_source.data()), size);
        m_source.resize(ifs.gcount());
        ifs.close();

        m_valid = true;
    }
    else
    {
        globjects::warning() << "Reading from file \"" << m_filePath << "\" failed.";

        m_source = "";

        m_valid = false;
    }
}

} // namespace globjects
