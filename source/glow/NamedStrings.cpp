#include "NamedStrings.h"

#include <cassert>
#include <algorithm>

#include <glow/Error.h>
#include <glow/StringSource.h>
#include <glow/String.h>
#include <glow/logging.h>
#include <glow/Version.h>
#include <glow/Extension.h>

namespace glow
{

NamedStrings* NamedStrings::s_instance = new NamedStrings;

NamedStrings::NamedStrings()
{
}

void NamedStrings::createNamedString(const std::string& name, const std::string& string, GLenum type)
{
    createNamedString(name, new String(string), type);
}

void NamedStrings::createNamedString(const std::string& name, StringSource* source, GLenum type)
{
    assert(source != nullptr);

    if (isNamedString(name, true))
    {
        if (s_instance->occurenceCount(source) <= 1 )
        {
            s_instance->m_registeredStringSources[name].source->deregisterListener(s_instance);
        }
    }

    s_instance->m_registeredStringSources[name].name = name;
    s_instance->m_registeredStringSources[name].source = source;
    s_instance->m_registeredStringSources[name].type = type;
    source->registerListener(s_instance);

    updateNamedString(s_instance->m_registeredStringSources[name]);
}

void NamedStrings::deleteNamedString(const std::string& name)
{
    if (isNamedString(name))
    {
        if (s_instance->occurenceCount(s_instance->m_registeredStringSources[name].source) <= 1)
        {
            s_instance->m_registeredStringSources[name].source->deregisterListener(s_instance);
        }

        s_instance->m_registeredStringSources.erase(name);
    }

    if (glow::hasExtension(GLOW_ARB_shading_language_include))
    {
        glDeleteNamedStringARB(static_cast<GLint>(name.size()), name.c_str());
        CheckGLError();
    }
}

bool NamedStrings::isNamedString(const std::string& name, bool cached)
{
    if (cached || !glow::hasExtension(GLOW_ARB_shading_language_include))
    {
        return s_instance->m_registeredStringSources.count(name) > 0;
    }

    bool result = glIsNamedStringARB(static_cast<GLint>(name.size()), name.c_str()) == GL_TRUE;
    CheckGLError();
    return result;
}

std::string NamedStrings::namedString(const std::string& name, bool cached)
{
    if (cached || !glow::hasExtension(GLOW_ARB_shading_language_include))
    {
        if (s_instance->m_registeredStringSources.find(name) == s_instance->m_registeredStringSources.end())
            return "";

        return s_instance->m_registeredStringSources[name].source->string();
    }

    GLint size = namedStringSize(name);
    GLchar* string = nullptr;

    glGetNamedStringARB(static_cast<GLint>(name.size()), name.c_str(), size, &size, string);
    CheckGLError();

    return std::string(string, size);
}

StringSource* NamedStrings::namedStringSource(const std::string& name)
{
    return s_instance->m_registeredStringSources[name].source;
}

GLint NamedStrings::namedStringSize(const std::string& name, bool cached)
{
    return namedStringParameter(name, GL_NAMED_STRING_LENGTH_ARB, cached);
}

GLenum NamedStrings::namedStringType(const std::string& name, bool cached)
{
    return static_cast<GLenum>(namedStringParameter(name, GL_NAMED_STRING_TYPE_ARB, cached));
}

GLint NamedStrings::namedStringParameter(const std::string& name, GLenum pname, bool cached)
{
    if (cached || !glow::hasExtension(GLOW_ARB_shading_language_include))
    {
        if (s_instance->m_registeredStringSources.find(name) == s_instance->m_registeredStringSources.end())
            return -1;

        switch (pname)
        {
            case GL_NAMED_STRING_LENGTH_ARB:
                return static_cast<GLint>(s_instance->m_registeredStringSources[name].source->string().size());
            case GL_NAMED_STRING_TYPE_ARB:
                return s_instance->m_registeredStringSources[name].type;
            default:
                return -1;
        }
    }

    int result;

    glGetNamedStringivARB(static_cast<GLint>(name.size()), name.c_str(), pname, &result);
    CheckGLError();

    return result;
}

void NamedStrings::updateNamedString(const std::string& name)
{
    if (!isNamedString(name))
    {
        return;
    }

    updateNamedString(s_instance->m_registeredStringSources[name]);
}

void NamedStrings::updateNamedString(const NamedString& namedString)
{
    if (!glow::hasExtension(GLOW_ARB_shading_language_include))
    {
        return;
    }

    std::string string = namedString.source->string();

    glNamedStringARB(namedString.type, static_cast<GLint>(namedString.name.size()), namedString.name.c_str(), static_cast<GLint>(string.size()), string.c_str());
    CheckGLError();
}

void NamedStrings::notifyChanged(Changeable* changed)
{
    for (const std::pair<std::string, NamedString>& pair : m_registeredStringSources)
    {
        if (pair.second.source.get() == changed)
        {
            updateNamedString(pair.second);
        }
    }
}

unsigned NamedStrings::occurenceCount(const StringSource* source)
{
    return static_cast<unsigned>(
        std::count_if(
            m_registeredStringSources.begin(), m_registeredStringSources.end(),
            [source](const std::pair<std::string, NamedString>& pair) {
                return pair.second.source == source;
            }
        )
    );
}


NamedStrings::NamedString::NamedString()
    : name("")
    , source(nullptr)
    , type(0)
{
}

} // namespace glow
