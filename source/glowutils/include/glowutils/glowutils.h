#pragma once

#include <string>
#include <vector>

#include <GL/glew.h>

#include <glowutils/glowutils_api.h>

namespace glow
{

class Shader;

}

namespace glowutils
{

GLOWUTILS_API glow::Shader * createShaderFromFile(GLenum type, const std::string & fileName);
GLOWUTILS_API glow::Shader * createShaderFromFile(GLenum type, const std::string & fileName, const std::vector<std::string> & includePaths);

GLOWUTILS_API void scanDirectory(const std::string & directory, const std::string & fileExtension);

} // namespace glowutils
