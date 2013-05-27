#include <glow/ShaderFile.h>
#include <glow/Shader.h>
#include <glow/internal/FileReader.h>

#include <iostream>

using namespace glow;

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

const std::string& ShaderFile::content() const
{
	return _fileContent;
}

void ShaderFile::registerShader(Shader* shader)
{
	_shaders.insert(shader);
}

void ShaderFile::deregisterShader(Shader* shader)
{
	_shaders.erase(shader);
}

void ShaderFile::reload()
{
	std::string backup = _fileContent;
	if (loadFileContent())
	{
		for (Shader* shader: _shaders)
		{
			bool previouslyCompiled = shader->isCompiled();
			shader->setSource(_fileContent);
			if (!shader->isCompiled() && previouslyCompiled)
			{
				shader->setSource(backup);
			}
		}
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
	if (!internal::FileReader::readFile(_filePath, _fileContent))
	{
		std::cerr << "Read from \"" << _filePath << "\" failed." << std::endl;
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
