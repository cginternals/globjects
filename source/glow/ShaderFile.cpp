
#include <glow/Shader.h>
#include <glow/RawFile.h>
#include <cassert>

#include <glow/ShaderFile.h>

namespace glow
{

ShaderFile::FileRegistry ShaderFile::s_fileRegistry;

ShaderFile::ShaderFile(const std::string & filePath)
:   m_filePath(filePath)
{
    RawFile<char> raw(m_filePath);
    if (raw.valid())
        m_source = std::string(raw.data(), raw.size());

	registerFile(this);
}

ShaderFile::~ShaderFile()
{
	deregisterFile(this);
}

const std::string & ShaderFile::source() const
{
	return m_source;
}

bool ShaderFile::isFile() const
{
	return true;
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

void ShaderFile::reloadAll()
{
	for (ShaderFile * file: s_fileRegistry)
		file->reload();
}

void ShaderFile::registerFile(ShaderFile * file)
{
    assert(file != nullptr);

	s_fileRegistry.insert(file);
}

void ShaderFile::deregisterFile(ShaderFile * file)
{
    assert(file != nullptr);

	s_fileRegistry.erase(file);
}

} // namespace glow
