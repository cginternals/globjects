#pragma once

#include <string>
#include <vector>

#include <GL/glew.h>

#include <glow/glow.h>
#include <glow/Version.h>


namespace glow 
{

/** \brief query provides an interface to query OpenGL.
    
    To query different types you must use the specific provided functions.
 */
class GLOW_API query
{
public:
	static std::string getString(GLenum pname);
	static GLint getInteger(GLenum pname);
	static GLfloat getFloat(GLenum pname);
	static GLdouble getDouble(GLenum pname);
	static GLboolean getBoolean(GLenum pname);

	static GLint getInteger(GLenum pname, unsigned index);
	static std::vector<GLint> getIntegers(GLenum pname, unsigned size);

	static std::string vendor();
	static std::string renderer();

	static Version version();
	static std::string versionString();

	static GLint majorVersion();
	static GLint minorVersion();
	static bool isCoreProfile();
};

/**
 * \brief memory provides an interface to query current memory stats of OpenGL.
 */
class GLOW_API memory
{
public:
	/** all sizes in kb */
	static GLint total();
	static GLint dedicated();
	static GLint available();
	static GLint evicted();
	static GLint evictionCount();

protected:
	static GLint memoryInfo(GLenum pname);
};

} // namespace glow
