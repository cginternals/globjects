#pragma once

#include <set>
#include <string>

#include <glow/glow.h>
#include <glow/ShaderSource.h>

namespace glow
{

class Shader;

/** \brief Shader source associated to a file.
    
    The file path of a ShaderFile can be queried using filePath(); To reload a
    shader from a file, use reload(). To reload all ShaderFiles, use reloadAll().

    \see ShaderSource
 */
class GLOW_API ShaderFile : public ShaderSource
{
public:
	ShaderFile(const std::string & filePath);
	virtual ~ShaderFile();

    virtual const std::string & source() const;
	virtual bool isFile() const;

	const std::string & filePath() const;

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
