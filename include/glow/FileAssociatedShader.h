
#pragma once

#include <set>
#include <unordered_map>

#include <glow/declspec.h>
#include <glow/Shader.h>


namespace glow
{

class GLOW_API FileAssociatedShader : public Shader
{
public:
    FileAssociatedShader(
        const GLenum type
    ,   const std::string & filePath);

    virtual ~FileAssociatedShader();

	void reload();

	static void reloadAll();

protected:
    const bool setSourceFromFile(const bool update = true);

    static void registerShader(
		const std::string & filePath
	,	FileAssociatedShader * shader);

    static void unregisterShader(
		const std::string & filePath
	,	FileAssociatedShader * shader);

protected:
	const std::string m_filePath;

protected:
	typedef std::set<FileAssociatedShader *> t_shaders;
    typedef std::unordered_map<std::string, t_shaders *> t_shadersByFilePath;

	static t_shadersByFilePath shadersByFilePath;
};

} // namespace glow