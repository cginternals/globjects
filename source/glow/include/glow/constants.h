#pragma once

#include <string>
#include <vector>

#include <glbinding/types.h>

#include <glow/glow_api.h>

namespace glow {

GLOW_API std::string enumName(gl::GLenum param);
GLOW_API std::vector<std::string> enumNames(gl::GLenum param);
	
} // namespace glow
