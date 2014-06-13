#pragma once

#include <string>

#include <glbinding/constants.h>

#include <glowbase/ChangeListener.h>
#include <glowbase/ref_ptr.h>
#include <glowbase/Referenced.h>
#include <glowbase/AbstractStringSource.h>

#include <glow/glow_api.h>

namespace glow {

class GLOW_API NamedString : public Referenced, protected ChangeListener
{
public:
    static NamedString * create(const std::string & name, AbstractStringSource * string, gl::GLenum type = gl::SHADER_INCLUDE_ARB);
    static NamedString * create(const std::string & name, const std::string & string, gl::GLenum type = gl::SHADER_INCLUDE_ARB);

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

    NamedString(const std::string & name, AbstractStringSource * source, gl::GLenum type = gl::SHADER_INCLUDE_ARB);

    virtual ~NamedString();

    void registerNamedString();
    void deregisterNamedString();
};

} // namespace glow
