#pragma once

#include <set>
#include <string>

#include <glow/declspec.h>
#include <glow/ShaderSource.h>

namespace glow
{

class Shader;

class GLOW_API ShaderFile : public ShaderSource
{
public:
	ShaderFile(const std::string& filePath);
	~ShaderFile();

	virtual const std::string& source();

	void reload();
	static void reloadAll();
protected:
	bool loadFileContent();
protected:
	std::string _filePath;
	std::string _fileContent;
protected:
	typedef std::set<ShaderFile*> FileRegistry;
	static FileRegistry _fileRegistry;

	static void registerFile(ShaderFile* file);
	static void deregisterFile(ShaderFile* file);
};

} // namespace glow
