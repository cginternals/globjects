#include <glow/NamedStrings.h>

#include <glow/Error.h>

namespace glow
{

NamedStrings::NamedStrings()
{
}

void NamedStrings::nameString(const std::string& name, const std::string& string, GLenum type)
{
    glNamedStringARB(type, name.size(), name.c_str(), string.size(), string.c_str());
    CheckGLError();
}

void NamedStrings::deleteNamedString(const std::string& name)
{
    glDeleteNamedStringARB(name.size(), name.c_str());
    CheckGLError();
}

bool NamedStrings::isNamedString(const std::string& name)
{
    bool result = glIsNamedStringARB(name.size(), name.c_str());
    CheckGLError();
    return result;
}

std::string NamedStrings::namedString(const std::string& name)
{
    GLint size = namedStringSize(name);
    GLchar* string;

    glGetNamedStringARB(name.size(), name.c_str(), size, &size, string);
    CheckGLError();

    return std::string(string, size);
}

GLint NamedStrings::namedStringSize(const std::string& name)
{
    return namedStringParameter(name, GL_NAMED_STRING_LENGTH_ARB);
}

GLenum NamedStrings::namedStringType(const std::string& name)
{
    return static_cast<GLenum>(namedStringParameter(name, GL_NAMED_STRING_TYPE_ARB));
}

GLint NamedStrings::namedStringParameter(const std::string& name, GLenum pname)
{
    int result;

    glGetNamedStringivARB(name.size(), name.c_str(), pname, &result);
    CheckGLError();

    return result;
}

} // namespace glow
