#include <GL/glew.h>

#include <glow/glow.h>
#include <glow/Error.h>

namespace glow
{

bool init()
{
	glewExperimental = GL_TRUE;
	
	if (glewInit() != GLEW_OK)
	{
		// possible errors:
		// GLEW_ERROR_NO_GL_VERSION
		// GLEW_ERROR_GL_VERSION_10_ONLY
		// GLEW_ERROR_GLX_VERSION_11_ONLY
		// TODO: handle?
		
		return false;
	}
	
	// NOTE: should be safe to ignore:
	// http://www.opengl.org/wiki/OpenGL_Loading_Library
	Error::clear(); // ignore GL_INVALID_ENUM
	
	return true;
}
	
} // namespace glow
