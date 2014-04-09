#pragma once

#include <string>
#include <unordered_map>

#include <GL/glew.h>

#include <glow/glow_api.h>
#include <glow/ref_ptr.h>
#include <glow/ChangeListener.h>

namespace glow 
{

class AbstractStringSource;

/** \brief Encapsulates OpenGL named strings.
    
    \see http://www.opengl.org/registry/specs/ARB/shading_language_include.txt
 */
class GLOW_API NamedStrings : protected ChangeListener
{
    struct NamedString
    {
        NamedString();

        std::string name;
        ref_ptr<AbstractStringSource> source;
        GLenum type;
    };
public:
    static void createNamedString(const std::string& name, const std::string& string, GLenum type = GL_SHADER_INCLUDE_ARB);
    static void createNamedString(const std::string& name, AbstractStringSource* source, GLenum type = GL_SHADER_INCLUDE_ARB);
    static void deleteNamedString(const std::string& name);
    static bool isNamedString(const std::string& name, bool cached = false);
    static std::string namedString(const std::string& name, bool cached = false);
    static AbstractStringSource* namedStringSource(const std::string& name);
    static GLenum namedStringType(const std::string& name, bool cached = false);

    virtual void notifyChanged(const Changeable* changed) override;
protected:
    static NamedStrings* s_instance;
    std::unordered_map<std::string, NamedString> m_registeredStringSources;
    NamedStrings();

    static void updateNamedString(const std::string& name);
    static void updateNamedString(const NamedString& namedString);
    static GLint namedStringSize(const std::string& name, bool cached = false);
    static GLint namedStringParameter(const std::string& name, GLenum pname, bool cached = false);

    unsigned occurenceCount(const AbstractStringSource* source);
};

} // namespace glow
