#pragma once

#include <set>
#include <string>

#include <GL/glew.h>

#include <glow/ShaderSource.h>

#include <glowutils/glowutils.h>

namespace glow
{

class Shader;

Shader* createShaderFromFile(GLenum type, const std::string& filename);

/** \brief Shader source associated to a file.
    
    The file path of a ShaderFile can be queried using filePath(); To reload a
    shader from a file, use reload(). To reload all ShaderFiles, use reloadAll().

    \see ShaderSource
 */
class GLOWUTILS_API ShaderFile : public ShaderSource
{
public:
	ShaderFile(const std::string & filePath);
	virtual ~ShaderFile();

    virtual const std::string & source() const override;
    virtual std::string shortInfo() const override;

	const std::string & filePath() const;

    void reload();
protected:
    std::string m_filePath;
    std::string m_source;
};

} // namespace glow
