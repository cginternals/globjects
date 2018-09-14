
#pragma once


#include <string>
#include <memory>
#include <set>

#include <glbinding/gl/types.h>

#include <globjects/base/Instantiator.h>

#include <globjects/globjects_api.h>


namespace globjects 
{


class AbstractStringSource;


class GLOBJECTS_API NamedString : public Instantiator<NamedString>
{
public:
    NamedString(const std::string & name, AbstractStringSource * source);
    NamedString(const std::string & name, AbstractStringSource * source, gl::GLenum type);

    virtual ~NamedString();

    static bool isNamedString(const std::string & name);
    static NamedString * getFromRegistry(const std::string & name);


public:
    const std::string & name() const;
    std::string string() const;
    gl::GLenum type() const;

    AbstractStringSource * stringSource() const;

    gl::GLint getParameter(gl::GLenum pname) const;

    virtual void notifyChanged(const AbstractStringSource * changeable);

    void addSubject(AbstractStringSource * subject);
    virtual void removeSubject(AbstractStringSource * subject);


protected:
    static bool hasNativeSupport();


protected:
    void updateString();

    void createNamedString();
    void deleteNamedString();

    void registerNamedString();
    void deregisterNamedString();


protected:
    std::set<AbstractStringSource*> m_sourceSubjects;

    std::string m_name;

    AbstractStringSource * m_source;
    gl::GLenum m_type;
};


} // namespace globjects
