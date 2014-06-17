#include <glow/NamedString.h>

#include <glbinding/functions.h>

#include <glowbase/StaticStringSource.h>

#include "registry/NamedStringRegistry.h"

namespace glow {

NamedString * NamedString::create(const std::string & name, AbstractStringSource * source, gl::GLenum type)
{
    if (isNamedString(name))
    {
        return nullptr;
    }

    return new NamedString(name, source, type);
}

NamedString * NamedString::create(const std::string & name, const std::string & string, gl::GLenum type)
{
    if (isNamedString(name))
    {
        return nullptr;
    }

    return new NamedString(name, new StaticStringSource(string), type);
}

NamedString::NamedString(const std::string & name, AbstractStringSource * source, gl::GLenum type)
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

    gl::glNamedStringARB(m_type, static_cast<gl::GLint>(m_name.size()), m_name.c_str(), static_cast<gl::GLint>(str.size()), str.c_str());
}

void NamedString::deleteNamedString()
{
    if (!hasNativeSupport())
        return;

    gl::glDeleteNamedStringARB(static_cast<gl::GLint>(m_name.size()), m_name.c_str());
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
        return gl::glIsNamedStringARB(static_cast<gl::GLint>(name.size()), name.c_str()) == gl::GL_TRUE;
    }

    return false;
}

gl::GLint NamedString::getParameter(gl::GLenum pname)
{
    if (hasNativeSupport())
    {
        gl::GLint result = 0;

        gl::glGetNamedStringivARB(static_cast<gl::GLint>(m_name.size()), m_name.c_str(), pname, &result);

        return result;
    }

    switch (pname)
    {
        case gl::GL_NAMED_STRING_LENGTH_ARB:
            return static_cast<gl::GLint>(string().size());
        case gl::GL_NAMED_STRING_TYPE_ARB:
            return static_cast<gl::GLint>(m_type);
        default:
            return -1;
    }
}

NamedString * NamedString::obtain(const std::string & name)
{
    NamedString * namedString = NamedStringRegistry::current().namedString(name);

    if (!namedString && hasNativeSupport() && isNamedString(name))
    {
        gl::GLint type;
        gl::GLint length;

        gl::glGetNamedStringivARB(static_cast<gl::GLint>(name.size()), name.c_str(), gl::GL_NAMED_STRING_TYPE_ARB, &type);
        gl::glGetNamedStringivARB(static_cast<gl::GLint>(name.size()), name.c_str(), gl::GL_NAMED_STRING_LENGTH_ARB, &length);

        std::vector<char> string(length);

        gl::glGetNamedStringARB(static_cast<gl::GLint>(name.size()), name.c_str(), length, nullptr, string.data());

        namedString = create(name, std::string(string.data(), string.size()), static_cast<gl::GLenum>(type));
    }

    return namedString;
}

const std::string & NamedString::name() const
{
    return m_name;
}

std::string NamedString::string() const
{
    return m_source->string();
}

gl::GLenum NamedString::type() const
{
    return m_type;
}

AbstractStringSource * NamedString::stringSource()
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


} // namespace glow
