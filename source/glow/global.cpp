
#include <GL/glew.h>

#include <glow/global.h>

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

std::string getString(GLenum pname)
{
	const GLubyte* result = glGetString(pname);
	CheckGLError();
	return reinterpret_cast<const char*>(result);
}

std::string getString(GLenum pname, GLuint index)
{
    const GLubyte* result = glGetStringi(pname, index);
    CheckGLError();
    return reinterpret_cast<const char*>(result);
}

GLint getInteger(GLenum pname)
{
	GLint value;

	glGetIntegerv(pname, &value);
	CheckGLError();

	return value;
}

GLfloat getFloat(GLenum pname)
{
	GLfloat value;

	glGetFloatv(pname, &value);
	CheckGLError();

	return value;
}

GLdouble getDouble(GLenum pname)
{
	GLdouble value;

	glGetDoublev(pname, &value);
	CheckGLError();

	return value;
}

GLboolean getBoolean(GLenum pname)
{
	GLboolean value;

	glGetBooleanv(pname, &value);
	CheckGLError();

	return value;
}

GLint getInteger(GLenum pname, GLuint index)
{
	GLint value;

	glGetIntegeri_v(pname, index, &value);
	CheckGLError();

	return value;
}

GLfloat getFloat(GLenum pname, GLuint index)
{
    GLfloat value;

    glGetFloati_v(pname, index, &value);
    CheckGLError();

    return value;
}

GLdouble getDouble(GLenum pname, GLuint index)
{
    GLdouble value;

    glGetDoublei_v(pname, index, &value);
    CheckGLError();

    return value;
}

GLboolean getBoolean(GLenum pname, GLuint index)
{
    GLboolean value;

    glGetBooleani_v(pname, index, &value);
    CheckGLError();

    return value;
}

} // namespace glow
