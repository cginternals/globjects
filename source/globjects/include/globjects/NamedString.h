#pragma once

#include <string>

#include <glbinding/gl/types.h>

#include <globjects-base/ChangeListener.h>
#include <globjects-base/ref_ptr.h>
#include <globjects-base/Referenced.h>
#include <globjects-base/AbstractStringSource.h>

#include <globjects/globjects_api.h>

namespace glo {

class GLOBJECTS_API NamedString : public Referenced, protected ChangeListener
{
public:
    static NamedString * create(const std::string & name, AbstractStringSource * string);
    static NamedString * create(const std::string & name, const std::string & string);

    static bool isNamedString(const std::string & name);
    static NamedString * obtain(const std::string & name);

    const std::string & name() const;
    std::string string() const;
    gl::GLenum type() const;

    AbstractStringSource * stringSource();

    gl::GLint getParameter(gl::GLenum pname);

    virtual void notifyChanged(const Changeable * changeable) override;
protected:
    std::string m_name;
    ref_ptr<AbstractStringSource> m_source;
    gl::GLenum m_type;

    static bool hasNativeSupport();

    void updateString();

    void createNamedString();
    void deleteNamedString();

    static NamedString * create(const std::string & name, AbstractStringSource * string, gl::GLenum type);
    static NamedString * create(const std::string & name, const std::string & string, gl::GLenum type);

    NamedString(const std::string & name, AbstractStringSource * source, gl::GLenum type);

    virtual ~NamedString();

    void registerNamedString();
    void deregisterNamedString();
};

} // namespace glo
