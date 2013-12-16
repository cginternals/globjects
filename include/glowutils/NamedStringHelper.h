#pragma once

#include <string>

#include <GL/glew.h>

#include <glow/StringSource.h>

#include <glowutils/glowutils.h>

namespace glowutils
{

class GLOWUTILS_API NamedStringHelper
{
public:
    static void scanDirectory(const std::string & directory, const std::string & fileExtension);
protected:
};

} // namespace glowutils
