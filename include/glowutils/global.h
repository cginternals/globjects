#pragma once

#include <string>

#include <GL/glew.h>

#include <glowutils/glowutils.h>

namespace glowutils
{

GLOWUTILS_API void scanDirectory(const std::string & directory, const std::string & fileExtension);

} // namespace glowutils
