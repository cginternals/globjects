#include <fstream>
#include <iostream>
#include <sstream>

#include <glow/FileAssociatedShader.h>
#include <glow/internal/FileReader.h>

using namespace glow;

FileAssociatedShader::ShaderRegistry FileAssociatedShader::_shaderRegistry;

FileAssociatedShader::FileAssociatedShader(GLenum type)
: Shader(type)
{
}

FileAssociatedShader::FileAssociatedShader(GLenum type, const std::string& filePath, bool compile)
: Shader(type)
{
	setSourceFile(filePath, compile);
}

FileAssociatedShader::~FileAssociatedShader()
{
	deregisterShader(this);
}

const std::string& FileAssociatedShader::filePath() const
{
	return _filePath;
}

void FileAssociatedShader::setSourceFile(const std::string& filePath, bool compile)
{
	deregisterShader(this);

	_filePath = filePath;
	loadSourceFile(compile);

	registerShader(this);
}

bool FileAssociatedShader::loadSourceFile(bool compile)
{
	std::string source;

	if (!internal::FileReader::readFile(_filePath, source))
	{
		std::cerr << "Read from \"" << _filePath << "\" failed." << std::endl;
		return false;
	}

	setSource(source, compile);

	return true;
}

void FileAssociatedShader::registerShader(FileAssociatedShader* shader)
{
	_shaderRegistry[shader->filePath()].insert(shader);
}

void FileAssociatedShader::deregisterShader(FileAssociatedShader* shader)
{
	_shaderRegistry[shader->filePath()].erase(shader);
}

void FileAssociatedShader::reloadAll()
{
	for (std::pair<std::string, std::set<FileAssociatedShader*>> pair: _shaderRegistry)
	{
		for (FileAssociatedShader* shader: pair.second)
		{
			shader->reload();
		}
	}
}

void FileAssociatedShader::reload()
{
	std::string backup = source();
	bool previousSourceWorks = isCompiled();

	if (loadSourceFile(true) && isCompiled())
	{
		return;
	}

	if (previousSourceWorks) setSource(backup, true);
}

