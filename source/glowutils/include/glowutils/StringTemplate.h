#pragma once

#include <string>
#include <map>

#include <glowutils/glowutils_api.h>
#include <glowutils/StringSourceDecorator.h>
#include <glowutils/CachedValue.h>

namespace glowutils 
{

class GLOWUTILS_API StringTemplate : public StringSourceDecorator
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
    CachedValue<std::string> m_modifiedSource;
	std::map<std::string, std::string> m_replacements;

    void invalidate();
    std::string modifiedSource() const;
};

} // namespace glowutils
