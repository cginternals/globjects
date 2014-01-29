#include <glow/global.h>

#include <glow/Error.h>
#include <glow/logging.h>

#include "NamedStrings.h"

namespace glow
{

bool glowIsInitialized = false;

bool initializeGLEW(bool showWarnings)
{
    glewExperimental = GL_TRUE;

    GLenum result = glewInit();
    if (result != GLEW_OK)
    {
        if (showWarnings)
        {
            glow::warning() << reinterpret_cast<const char*>(glewGetErrorString(result));
        }

        return false;
    }

    // NOTE: should be safe to ignore:
    // http://www.opengl.org/wiki/OpenGL_Loading_Library
    glow::Error::clear(); // ignore GL_INVALID_ENUM

    return true;
}

bool isInitialized()
{
    return glowIsInitialized;
}

bool init(bool showWarnings)
{
    if (glowIsInitialized)
    {
        if (showWarnings)
        {
            glow::warning() << "glow is already initialized";
        }

        return true;
    }

    if (!initializeGLEW(showWarnings))
    {
        return false;
    }

    glowIsInitialized = true;

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

std::string vendor()
{
    return getString(GL_VENDOR);
}

std::string renderer()
{
    return getString(GL_RENDERER);
}

std::string versionString()
{
    return getString(GL_VERSION);
}

GLint majorVersion()
{
    return getInteger(GL_MAJOR_VERSION);
}

GLint minorVersion()
{
    return getInteger(GL_MINOR_VERSION);
}

Version version()
{
    return Version(majorVersion(), minorVersion());
}

bool isCoreProfile()
{
    if (version()<Version(3,2))
    {
        return false;
    }

    return (getInteger(GL_CONTEXT_PROFILE_MASK) & GL_CONTEXT_CORE_PROFILE_BIT) > 0;
}

std::vector<std::string> getExtensions()
{
    int count = getInteger(GL_NUM_EXTENSIONS);

    std::vector<std::string> extensions(count);

    for (int i=0; i<count; ++i)
    {
        extensions[i] = getString(GL_EXTENSIONS, i);
    }

    return extensions;
}

void createNamedString(const std::string& name, const std::string& string, GLenum type)
{
    NamedStrings::createNamedString(name, string, type);
}

void createNamedString(const std::string& name, AbstractStringSource* source, GLenum type)
{
    NamedStrings::createNamedString(name, source, type);
}

void deleteNamedString(const std::string& name)
{
    NamedStrings::deleteNamedString(name);
}

bool isNamedString(const std::string& name, bool cached)
{
    return NamedStrings::isNamedString(name, cached);
}

std::string getNamedString(const std::string& name, bool cached)
{
    return NamedStrings::namedString(name, cached);
}

AbstractStringSource* getNamedStringSource(const std::string& name)
{
    return NamedStrings::namedStringSource(name);
}

GLenum getNamedStringType(const std::string& name, bool cached)
{
    return NamedStrings::namedStringType(name, cached);
}

void enable(GLenum capability)
{
    glEnable(capability);
    CheckGLError();
}

void disable(GLenum capability)
{
    glDisable(capability);
    CheckGLError();
}

bool isEnabled(GLenum capability)
{
    GLboolean value = glIsEnabled(capability);
    CheckGLError();
    return value == GL_TRUE;
}

void setEnabled(GLenum capability, bool enabled)
{
    enabled ? enable(capability) : disable(capability);
}

void enable(GLenum capability, int index)
{
    glEnablei(capability, index);
    CheckGLError();
}

void disable(GLenum capability, int index)
{
    glDisablei(capability, index);
    CheckGLError();
}

bool isEnabled(GLenum capability, int index)
{
    GLboolean value = glIsEnabledi(capability, index);
    CheckGLError();
    return value == GL_TRUE;
}

void setEnabled(GLenum capability, int index, bool enabled)
{
    enabled ? enable(capability, index) : disable(capability, index);
}

} // namespace glow
