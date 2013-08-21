
#include <glow/ShaderFile.h>
#include <glow/Shader.h>
#include <glow/logging.h>
#include "FileReader.h"

namespace glow
{

ShaderFile::FileRegistry ShaderFile::_fileRegistry;

ShaderFile::ShaderFile(const std::string& filePath)
: _filePath(filePath)
{
	registerFile(this);
	loadFileContent();
}

ShaderFile::~ShaderFile()
{
	deregisterFile(this);
}

const std::string& ShaderFile::source()
{
	return _fileContent;
}

void ShaderFile::reload()
{
	if (loadFileContent())
	{
		changed();
	}
}

void ShaderFile::reloadAll()
{
	for (ShaderFile* file: _fileRegistry)
	{
		file->reload();
	}
}

bool ShaderFile::loadFileContent()
{
	if (!FileReader::readFile(_filePath, _fileContent))
	{
		critical() << "Reading from file \"" << _filePath << "\" failed.";
		return false;
	}

	return true;
}

void ShaderFile::registerFile(ShaderFile* file)
{
	_fileRegistry.insert(file);
}

void ShaderFile::deregisterFile(ShaderFile* file)
{
	_fileRegistry.erase(file);
}

} // namespace glow
