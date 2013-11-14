#include <glow/NamedStrings.h>

#include <glow/Error.h>
#include <glow/StringSource.h>
#include <glow/String.h>

namespace glow
{

NamedStrings NamedStrings::s_instance;

NamedStrings::NamedStrings()
{
}

void NamedStrings::createNamedString(const std::string& name, const std::string& string, GLenum type)
{
    createNamedString(name, new String(string), type);
}

void NamedStrings::createNamedString(const std::string& name, StringSource* source, GLenum type)
{
    if (isNamedString(name))
    {
        if (s_instance.sourceOccurenceCount(source) <= 1 )
        {
            s_instance.m_registeredStringSources[name].source->deregisterListener(&s_instance);
        }
    }

    s_instance.m_registeredStringSources[name].name = name;
    s_instance.m_registeredStringSources[name].source = source;
    s_instance.m_registeredStringSources[name].type = type;
    source->registerListener(&s_instance);

    updateNamedString(s_instance.m_registeredStringSources[name]);
}

void NamedStrings::deleteNamedString(const std::string& name)
{
    if (isNamedString(name))
    {
        if (s_instance.sourceOccurenceCount(s_instance.m_registeredStringSources[name].source) <= 1)
        {
            s_instance.m_registeredStringSources[name].source->deregisterListener(&s_instance);
        }

        s_instance.m_registeredStringSources.erase(name);
    }

    glDeleteNamedStringARB(name.size(), name.c_str());
    CheckGLError();
}

bool NamedStrings::isNamedString(const std::string& name, bool cached)
{
    if (cached)
    {
        return s_instance.m_registeredStringSources.count(name) > 0;
    }

    bool result = glIsNamedStringARB(name.size(), name.c_str()) == GL_TRUE;
    CheckGLError();
    return result;
}

std::string NamedStrings::namedString(const std::string& name, bool cached)
{
    if (cached)
    {
        return s_instance.m_registeredStringSources[name].source->string();
    }

    GLint size = namedStringSize(name);
    GLchar* string = nullptr;

    glGetNamedStringARB(name.size(), name.c_str(), size, &size, string);
    CheckGLError();

    return std::string(string, size);
}

GLint NamedStrings::namedStringSize(const std::string& name, bool cached)
{
    if (cached)
    {
        return s_instance.m_registeredStringSources[name].source->string().size();
    }

    return namedStringParameter(name, GL_NAMED_STRING_LENGTH_ARB);
}

GLenum NamedStrings::namedStringType(const std::string& name, bool cached)
{
    if (cached)
    {
        return s_instance.m_registeredStringSources[name].type;
    }

    return static_cast<GLenum>(namedStringParameter(name, GL_NAMED_STRING_TYPE_ARB));
}

GLint NamedStrings::namedStringParameter(const std::string& name, GLenum pname)
{
    int result;

    glGetNamedStringivARB(name.size(), name.c_str(), pname, &result);
    CheckGLError();

    return result;
}

void NamedStrings::updateNamedString(const std::string& name)
{
    if (!isNamedString(name))
    {
        return;
    }

    updateNamedString(s_instance.m_registeredStringSources[name]);
}

void NamedStrings::updateNamedString(const NamedString& namedString)
{
    if (namedString.name == "")
    {
        return;
    }

    std::string string = namedString.source->string();

    glNamedStringARB(namedString.type, namedString.name.size(), namedString.name.c_str(), string.size(), string.c_str());
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

unsigned NamedStrings::sourceOccurenceCount(const StringSource* source)
{
    unsigned count = 0;

    for (const std::pair<std::string, NamedString>& pair : m_registeredStringSources)
    {
        if (pair.second.source == source)
        {
            ++count;
        }
    }

    return count;
}


NamedStrings::NamedString::NamedString()
    : name("")
    , source(nullptr)
    , type(0)
{
}

} // namespace glow
