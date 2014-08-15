#pragma once

#include <string>
#include <map>

#include <globjects-base/CachedValue.h>

#include <globjects-utils/globjects-utils_api.h>
#include <globjects-utils/StringSourceDecorator.h>

namespace gloutils 
{

class GLOBJECTS_UTILS_API StringTemplate : public StringSourceDecorator
{
public:
    StringTemplate(AbstractStringSource * source);

    virtual std::string string() const override;
    virtual void update() override;

    void replace(const std::string & original, const std::string & str);
    void replace(const std::string & original, int i);

    void clearReplacements();

protected:
    glo::CachedValue<std::string> m_modifiedSource;
	std::map<std::string, std::string> m_replacements;

    virtual ~StringTemplate();

    void invalidate();
    std::string modifiedSource() const;
};

} // namespace gloutils
