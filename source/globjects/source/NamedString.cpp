
#include <globjects/NamedString.h>

#include <glbinding/gl/functions.h>
#include <glbinding/gl/boolean.h>
#include <glbinding/gl/enum.h>

#include <globjects/base/AbstractStringSource.h>
#include <globjects/base/StaticStringSource.h>

#include "registry/NamedStringRegistry.h"

using namespace gl;


namespace globjects 
{


NamedString::NamedString(const std::string & name, AbstractStringSource * source)
: NamedString(name, source, GL_SHADER_INCLUDE_ARB)
{
}

NamedString::NamedString(const std::string & name, AbstractStringSource * source, const GLenum type)
: m_name(name)
, m_source(source)
, m_type(type)
{
    createNamedString();
    registerNamedString();

    m_source->registerListener(this);
}

NamedString::~NamedString()
{
    m_source->deregisterListener(this);

    deregisterNamedString();
    deleteNamedString();
}

void NamedString::createNamedString()
{
    if (!hasNativeSupport())
        return;

    std::string str = string();

    glNamedStringARB(m_type, static_cast<GLint>(m_name.size()), m_name.c_str(), static_cast<GLint>(str.size()), str.c_str());
}

void NamedString::deleteNamedString()
{
    if (!hasNativeSupport())
        return;

    glDeleteNamedStringARB(static_cast<GLint>(m_name.size()), m_name.c_str());
}

void NamedString::registerNamedString()
{
    NamedStringRegistry::current().registerNamedString(this);
}

void NamedString::deregisterNamedString()
{
    NamedStringRegistry::current().deregisterNamedString(this);
}

bool NamedString::isNamedString(const std::string & name)
{
    if (NamedStringRegistry::current().hasNamedString(name))
    {
        return true;
    }

    if (hasNativeSupport())
    {
        return glIsNamedStringARB(static_cast<GLint>(name.size()), name.c_str()) == GL_TRUE;
    }

    return false;
}

GLint NamedString::getParameter(const GLenum pname) const
{
    if (hasNativeSupport())
    {
        GLint result = 0;

        glGetNamedStringivARB(static_cast<GLint>(m_name.size()), m_name.c_str(), pname, &result);

        return result;
    }

    switch (pname)
    {
        case GL_NAMED_STRING_LENGTH_ARB:
            return static_cast<GLint>(string().size());
        case GL_NAMED_STRING_TYPE_ARB:
            return static_cast<GLint>(m_type);
        default:
            return -1;
    }
}

NamedString * NamedString::getFromRegistry(const std::string & name)
{
    return NamedStringRegistry::current().namedString(name);
}

const std::string & NamedString::name() const
{
    return m_name;
}

std::string NamedString::string() const
{
    return m_source->string();
}

GLenum NamedString::type() const
{
    return m_type;
}

AbstractStringSource * NamedString::stringSource() const
{
    return m_source;
}

bool NamedString::hasNativeSupport()
{
    return NamedStringRegistry::current().hasNativeSupport();
}

void NamedString::updateString()
{
    createNamedString();
}

void NamedString::notifyChanged(const Changeable *)
{
    updateString();
}


} // namespace globjects
