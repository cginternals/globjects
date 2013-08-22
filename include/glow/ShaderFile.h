#pragma once

#include <set>
#include <string>

#include <glow/glow.h>
#include <glow/ShaderSource.h>

namespace glow
{

class Shader;

class GLOW_API ShaderFile : public ShaderSource
{
public:
	ShaderFile(const std::string & filePath);
	virtual ~ShaderFile();

	virtual const std::string & source();

	void reload();
	static void reloadAll();

protected:
    static void registerFile(ShaderFile * file);
	static void deregisterFile(ShaderFile * file);

protected:
	typedef std::set<ShaderFile*> FileRegistry;
	static FileRegistry s_fileRegistry;

protected:
    std::string m_filePath;
    std::string m_source;
};

} // namespace glow
