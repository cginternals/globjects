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
	FileAssociatedShader(GLenum type);
	FileAssociatedShader(GLenum type, const std::string& filePath, bool compile = false);

	virtual ~FileAssociatedShader();

	void setSourceFile(const std::string& filePath, bool compile = false);
	const std::string& filePath() const;

	void reload();
	static void reloadAll();
protected:
	bool loadSourceFile(bool compile);

	static void registerShader(FileAssociatedShader* shader);
	static void deregisterShader(FileAssociatedShader* shader);
protected:
	std::string _filePath;
protected:
	typedef std::unordered_map<std::string, std::set<FileAssociatedShader*>> ShaderRegistry;
	static ShaderRegistry _shaderRegistry;
};

} // namespace glow
