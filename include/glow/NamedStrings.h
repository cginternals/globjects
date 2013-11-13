#pragma once

#include <string>

#include <glow/glow.h>

#include <glow/Array.h>
#include <glow/Object.h>

namespace glow 
{

/** \brief Encapsulates OpenGL named strings.
    
    \see http://www.opengl.org/registry/specs/ARB/shading_language_include.txt
 */
class GLOW_API NamedStrings
{
public:
    static void nameString(const std::string& name, const std::string& string, GLenum type = GL_SHADER_INCLUDE_ARB);
    static void deleteNamedString(const std::string& name);
    static bool isNamedString(const std::string& name);
    static std::string namedString(const std::string& name);
protected:
    NamedStrings();

    static GLint namedStringSize(const std::string& name);
    static GLenum namedStringType(const std::string& name);
    static GLint namedStringParameter(const std::string& name, GLenum pname);
};

} // namespace glow
