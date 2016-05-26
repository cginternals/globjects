
#pragma once

#include <string>

#include <glbinding/gl/types.h>

#include <globjects/base/ChangeListener.h>
#include <globjects/base/ref_ptr.h>
#include <globjects/base/Referenced.h>

#include <globjects/globjects_api.h>


namespace globjects 
{


class AbstractStringSource;

class GLOBJECTS_API NamedString : public Referenced, protected ChangeListener
{
public:
    static NamedString * create(const std::string & name, AbstractStringSource * string);
    static NamedString * create(const std::string & name, const std::string & string);

    static bool isNamedString(const std::string & name);
    static NamedString * obtain(const std::string & name);

public:
    const std::string & name() const;
    std::string string() const;
    gl::GLenum type() const;

    AbstractStringSource * stringSource() const;

    gl::GLint getParameter(gl::GLenum pname) const;

    virtual void notifyChanged(const Changeable * changeable) override;

protected:
    static bool hasNativeSupport();

    static NamedString * create(const std::string & name, AbstractStringSource * string, gl::GLenum type);
    static NamedString * create(const std::string & name, const std::string & string, gl::GLenum type);

protected:
    void updateString();

    void createNamedString();
    void deleteNamedString();

    NamedString(const std::string & name, AbstractStringSource * source, gl::GLenum type);

    virtual ~NamedString();

    void registerNamedString();
    void deregisterNamedString();

protected:
    std::string m_name;

    ref_ptr<AbstractStringSource> m_source;
    gl::GLenum m_type;
};


} // namespace globjects
