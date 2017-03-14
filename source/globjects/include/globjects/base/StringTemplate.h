
#pragma once

#include <string>
#include <map>

#include <globjects/globjects_api.h>

#include <globjects/base/AbstractStringSourceDecorator.h>
#include <globjects/base/Instantiator.h>


namespace globjects
{


class GLOBJECTS_API StringTemplate : public AbstractStringSourceDecorator, public Instantiator<StringTemplate>
{
public:
    StringTemplate(AbstractStringSource * source);
    virtual ~StringTemplate();

    virtual std::string string() const override;
    virtual void update() override;

    void replace(const std::string & original, const std::string & str);
    void replace(const std::string & original, int i);

    void clearReplacements();

protected:
    mutable std::string m_modifiedSource;
    mutable bool m_modifiedSourceValid;

    std::map<std::string, std::string> m_replacements;

    void invalidate();
    std::string modifiedSource() const;
};


} // namespace globjects
