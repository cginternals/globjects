
#include <GL/glew.h>

#include <glow/glow.h>

#include <string>
#include <vector>

namespace glow {

GLOW_API std::string enumName(GLenum param);
GLOW_API std::vector<std::string> enumNames(GLenum param);
	
} // namespace glow
