
#pragma once

#include <string>
#include <map>

#include <globjects/globjects_api.h>

#include <globjects/base/StringSourceDecorator.h>


namespace globjects
{


class GLOBJECTS_API StringTemplate : public StringSourceDecorator
{
public:
    StringTemplate(AbstractStringSource * source);

    virtual std::string string() const override;
    virtual void update() override;

    void replace(const std::string & original, const std::string & str);
    void replace(const std::string & original, int i);

    void clearReplacements();

protected:
    mutable std::string m_modifiedSource;
    mutable bool m_modifiedSourceValid;

	std::map<std::string, std::string> m_replacements;

    virtual ~StringTemplate();

    void invalidate();
    std::string modifiedSource() const;
};


} // namespace globjects
