
#include <glow/ShaderFile.h>
#include <glow/Shader.h>
#include <glow/RawFile.h>


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

const std::string & ShaderFile::source()
{
	return m_source;
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
	s_fileRegistry.insert(file);
}

void ShaderFile::deregisterFile(ShaderFile * file)
{
	s_fileRegistry.erase(file);
}

} // namespace glow
