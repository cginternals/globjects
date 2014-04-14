#pragma once

#include <GL/glew.h>

#include <string>
#include <vector>

#include <glow/glow_api.h>

namespace glow {

GLOW_API std::string enumName(GLenum param);
GLOW_API std::vector<std::string> enumNames(GLenum param);
	
} // namespace glow
